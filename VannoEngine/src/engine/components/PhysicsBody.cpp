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
#include "engine/systems/physics/Collision.h"

#include "engine/systems/graphics/GraphicsManager.h"

#include "engine/util/bitmask.h"

#include <math.h>
#include <algorithm>
namespace VannoEngine {
	PhysicsBody::PhysicsBody(GameObject* owner) :
		GameComponent(owner),
		mAabbOffset(glm::vec2(0.0f, 0.0f)),
		mHitUp(false),
		mHitDown(false),
		mHitRight(false),
		mHitLeft(false)
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
			mOldPosition = position;
			glm::vec2 speed = pTransform->GetSpeed();
			mOldSpeed = speed;

			// Update AABB
			mAabb.center = position + mAabbOffset;

			// Update Speed
			speed.y += pPhysicsManager->GetGravity();
			if (speed.x > -1.0f && speed.x < 1.0f) {
				speed.x = 0.0f;
			}

			// Update position
			position += speed * dT;

			// Check to see if on the ground
			Collision collisionData = pPhysicsManager->CollidesWithMap(mAabb);
			/*
			LOG_CORE_DEBUG("Collision detected ({0}): {1} {2} {3} {4} {5}",
				collisionMask,
				(collisionMask == 0 ? "NONE" : ""),
				(IsSet(collisionMask, BIT_COLLISION_TOP) ? "TOP" : ""),
				(IsSet(collisionMask, BIT_COLLISION_RIGHT) ? "RIGHT" : ""), 
				(IsSet(collisionMask, BIT_COLLISION_BOTTOM) ? "BOTTOM" : ""), 
				(IsSet(collisionMask, BIT_COLLISION_LEFT) ? "LEFT" : ""));
			*/

			if (IsSet(collisionData.bits, BIT_COLLISION_TOP)) {
				mHitUp = true;
			}
			else {
				mHitUp = false;
			}
			
			if (IsSet(collisionData.bits, BIT_COLLISION_BOTTOM) && speed.y < 0.0f) {
				position.y = mOldPosition.y;
				mHitDown = true;
			}
			else {
				mHitDown = false;
			}

			if (IsSet(collisionData.bits, BIT_COLLISION_LEFT)) {
				mHitLeft = true;
			}
			else {
				mHitLeft = false;
			}

			if (IsSet(collisionData.bits, BIT_COLLISION_RIGHT)) {
				mHitRight = true;
			}
			else {
				mHitRight = false;
			}
			//LOG_CORE_DEBUG("On ground? {0}", mOnGround);

			pTransform->SetPosition(position.x, position.y);
			pTransform->SetSpeed(speed.x, speed.y);
		}
	}

	void PhysicsBody::Draw() {
		GraphicsManager* pGraphicsManager = GraphicsManager::GetInstance();
		glm::vec4 yellow(1.0f, 1.0f, 0.0f, 1.0f);
		pGraphicsManager->RenderSquare(mAabb.center, mAabb.halfWidth * 2.0f, mAabb.halfHeight * 2.0f, glm::vec4(1.0, 0.0f, 0.0f, 1.0f), false);
		
		if (mHitUp) {
			glm::vec2 start(mAabb.center.x - mAabb.halfWidth, mAabb.center.y + mAabb.halfHeight);
			glm::vec2 end(mAabb.center.x + mAabb.halfWidth, mAabb.center.y + mAabb.halfHeight);

			pGraphicsManager->RenderLine(start, end, yellow);
		}

		if(mHitDown) {
			glm::vec2 start(mAabb.center.x - mAabb.halfWidth, mAabb.center.y - mAabb.halfHeight);
			glm::vec2 end(mAabb.center.x + mAabb.halfWidth, mAabb.center.y - mAabb.halfHeight);

			pGraphicsManager->RenderLine(start, end, yellow);
		}

		if (mHitLeft) {
			glm::vec2 start(mAabb.center.x - mAabb.halfWidth, mAabb.center.y + mAabb.halfHeight);
			glm::vec2 end(mAabb.center.x - mAabb.halfWidth, mAabb.center.y - mAabb.halfHeight);

			pGraphicsManager->RenderLine(start, end, yellow);
		}

		if (mHitRight) {
			glm::vec2 start(mAabb.center.x + mAabb.halfWidth, mAabb.center.y + mAabb.halfHeight);
			glm::vec2 end(mAabb.center.x + mAabb.halfWidth, mAabb.center.y - mAabb.halfHeight);

			pGraphicsManager->RenderLine(start, end, yellow);
		}
	}
}