/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		PhysicsBody.cpp
Purpose:		Component to describe a physics body
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-01
*************************************************************************/

#include "PhysicsBody.h"

#include "engine/core/Log.h"

#include "Transform.h"

#include "engine/systems/objects/GameObject.h"

#include "engine/systems/physics/PhysicsManager.h"

#include "engine/systems/graphics/GraphicsManager.h"
#include "engine/systems/graphics/Colors.h"

#include "engine/systems/ConfigurationManager.h"

#include "engine/util/bitmask.h"

#include <math.h>
#include <algorithm>
namespace VannoEngine {
	PhysicsBody::PhysicsBody(GameObject* owner) :
		GameComponent(owner),
		mAabbOffset(glm::vec2(0.0f, 0.0f))
	{ }

	PhysicsBody::~PhysicsBody() {

	}
	void PhysicsBody::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
		float x = 0.0f;
		if (pData->HasMember("offsetX") && (*pData)["offsetX"].IsFloat()) {
			x = (*pData)["offsetX"].GetFloat();
		}

		float y = 0.0f;
		if (pData->HasMember("offsetY") && (*pData)["offsetY"].IsFloat()) {
			y = (*pData)["offsetY"].GetFloat();
		}
		mAabbOffset = glm::vec2(x, y);

		if (pData->HasMember("width") && (*pData)["width"].IsFloat()) {
			mAabb.halfWidth = (*pData)["width"].GetFloat() / 2.0f;
		}

		if (pData->HasMember("height") && (*pData)["height"].IsFloat()) {
			mAabb.halfHeight = (*pData)["height"].GetFloat() / 2.0f;
		}
	}

	void PhysicsBody::Update(double deltaTime) {
		float dT = (float)deltaTime;
		PhysicsManager* pPhysicsManager = PhysicsManager::GetInstance();
		GameObject* pObject = GetOwner();
		Transform* pTransform = static_cast<Transform*>(pObject->GetComponent(TRANSFORM_COMPONENT));

		if (pTransform) {
			// Update old values
			glm::vec2 position = pTransform->GetPosition();
			glm::vec2 speed = pTransform->GetSpeed();

			// Update Speed
			speed.y += pPhysicsManager->GetGravity();
			if (speed.x > -1.0f && speed.x < 1.0f) {
				speed.x = 0.0f;
			}

			// Update position
			position += speed * dT;

			// Update AABB
			mAabb.center = position + mAabbOffset;

			mCollision = pPhysicsManager->CollidesWithMap(mAabb);

			pTransform->SetPosition(position.x, position.y);
			pTransform->SetSpeed(speed.x, speed.y);
		}
	}

	void PhysicsBody::Draw() {
		if(ConfigurationManager::GetInstance()->GetBool("/debugMode")) {
			GraphicsManager* pGraphicsManager = GraphicsManager::GetInstance();
		
			//pGraphicsManager->RenderSquare(mAabb.center, mAabb.halfWidth * 2.0f, mAabb.halfHeight * 2.0f, glm::vec4(1.0, 0.0f, 0.0f, 1.0f), false);
			//pGraphicsManager->RenderCircle(mAabb.center, 3.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), true);
		
			if (mCollision.GetType(Direction::TOP) == CollisionType::HARD) {
				glm::vec2 start(mAabb.center.x - mAabb.halfWidth + mAabb.margin, mAabb.center.y + mAabb.halfHeight);
				glm::vec2 end(mAabb.center.x + mAabb.halfWidth - mAabb.margin, mAabb.center.y + mAabb.halfHeight);

				pGraphicsManager->RenderLine(start, end, YELLOW);
				pGraphicsManager->RenderLine(glm::vec2(start.x-100.0f, mCollision.GetEdge(Direction::TOP)), glm::vec2(end.x + 100.0f, mCollision.GetEdge(Direction::TOP)), PURPLE);
			}

			if(mCollision.GetType(Direction::BOTTOM) == CollisionType::HARD) {
				glm::vec2 start(mAabb.center.x - mAabb.halfWidth + mAabb.margin, mAabb.center.y - mAabb.halfHeight);
				glm::vec2 end(mAabb.center.x + mAabb.halfWidth - mAabb.margin, mAabb.center.y - mAabb.halfHeight);

				pGraphicsManager->RenderLine(start, end, YELLOW);
				pGraphicsManager->RenderLine(glm::vec2(start.x-100.0f, mCollision.GetEdge(Direction::BOTTOM)), glm::vec2(end.x + 100.0f, mCollision.GetEdge(Direction::BOTTOM)), GREEN);
			}

			if (mCollision.GetType(Direction::LEFT) == CollisionType::HARD) {
				glm::vec2 start(mAabb.center.x - mAabb.halfWidth, mAabb.center.y + mAabb.halfHeight - mAabb.margin);
				glm::vec2 end(mAabb.center.x - mAabb.halfWidth, mAabb.center.y - mAabb.halfHeight + mAabb.margin);

				pGraphicsManager->RenderLine(start, end, RED);
				pGraphicsManager->RenderLine(glm::vec2(mCollision.GetEdge(Direction::LEFT), start.y - 100.0f), glm::vec2(mCollision.GetEdge(Direction::LEFT), end.y + 100.0f), RED);
			}

			if (mCollision.GetType(Direction::RIGHT) == CollisionType::HARD) {
				glm::vec2 start(mAabb.center.x + mAabb.halfWidth, mAabb.center.y + mAabb.halfHeight - mAabb.margin);
				glm::vec2 end(mAabb.center.x + mAabb.halfWidth, mAabb.center.y - mAabb.halfHeight + mAabb.margin);

				pGraphicsManager->RenderLine(start, end, BLUE);
				pGraphicsManager->RenderLine(glm::vec2(mCollision.GetEdge(Direction::RIGHT), start.y - 100.0f), glm::vec2(mCollision.GetEdge(Direction::RIGHT), end.y + 100.0f), BLUE);
			}
		}
	}
}