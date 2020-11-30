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
#include "engine/systems/physics/ObjectCollisionEvent.h"
#include "engine/systems/physics/MapCollisionEvent.h"

#include "engine/systems/graphics/GraphicsManager.h"
#include "engine/systems/graphics/Colors.h"

#include "engine/systems/events/EventManager.h"

#include "engine/systems/ConfigurationManager.h"

#include <math.h>
#include <algorithm>
namespace VannoEngine {
	PhysicsBody::PhysicsBody(GameObject* owner) :
		GameComponent(owner),
		mAabbOffset{ glm::vec2(0.0f, 0.0f) },
		mMass{ 1.0f },
		mPhysicsLayer{ "default" }
	{ 
		EventManager::GetInstance()->Subscribe(EVT_MAP_COLLISION, this);
		mSoftCollide.Set(static_cast<int>(Direction::DOWN));
	}

	PhysicsBody::~PhysicsBody() {
		EventManager::GetInstance()->Unsubscribe(EVT_MAP_COLLISION, this);
	}
	void PhysicsBody::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
		float x = 0.0f;
		if (pData->HasMember("offsetX") && (*pData)["offsetX"].IsNumber()) {
			x = (*pData)["offsetX"].GetFloat();
		}

		float y = 0.0f;
		if (pData->HasMember("offsetY") && (*pData)["offsetY"].IsNumber()) {
			y = (*pData)["offsetY"].GetFloat();
		}
		mAabbOffset = glm::vec2(x, y);

		if (pData->HasMember("width") && (*pData)["width"].IsNumber()) {
			mAabb.halfWidth = (*pData)["width"].GetFloat() / 2.0f;
		}

		if (pData->HasMember("height") && (*pData)["height"].IsNumber()) {
			mAabb.halfHeight = (*pData)["height"].GetFloat() / 2.0f;
		}

		if (pData->HasMember("mass") && (*pData)["mass"].IsNumber()) {
			mMass = (*pData)["mass"].GetFloat();
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
			speed.y += pPhysicsManager->GetGravity() * mMass * (float)deltaTime;

			// Update position
			position += speed;

			pPhysicsManager->CheckCollisions(this);
			
			pTransform->SetPosition(position.x, position.y);
			pTransform->SetSpeed(speed.x, speed.y);
		}
	}

	void PhysicsBody::Draw() {
		if(ConfigurationManager::GetInstance()->GetBool("/debugMode")) {
			GraphicsManager* pGraphicsManager = GraphicsManager::GetInstance();
			Transform* pTransform = static_cast<Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));

			if(pTransform) {
				glm::vec2 center = pTransform->GetPosition() + mAabbOffset;
				pGraphicsManager->RenderSquare(center, mAabb.halfWidth * 2.0f, mAabb.halfHeight * 2.0f, RED, false);
				pGraphicsManager->RenderCircle(center, 3.0f, RED, true);
		//
		//		if (mCollision.GetType(Direction::TOP) == CollisionType::HARD) {
		//			glm::vec2 start(center.x - mAabb.halfWidth + mAabb.margin, center.y + mAabb.halfHeight);
		//			glm::vec2 end(center.x + mAabb.halfWidth - mAabb.margin, center.y + mAabb.halfHeight);

		//			pGraphicsManager->RenderLine(start, end, YELLOW);
		//			pGraphicsManager->RenderLine(glm::vec2(start.x-100.0f, mCollision.GetEdge(Direction::TOP)), glm::vec2(end.x + 100.0f, mCollision.GetEdge(Direction::TOP)), PURPLE);
		//		}

		//		if(mCollision.GetType(Direction::BOTTOM) == CollisionType::HARD) {
		//			glm::vec2 start(center.x - mAabb.halfWidth + mAabb.margin, center.y - mAabb.halfHeight);
		//			glm::vec2 end(center.x + mAabb.halfWidth - mAabb.margin, center.y - mAabb.halfHeight);

		//			pGraphicsManager->RenderLine(start, end, YELLOW);
		//			pGraphicsManager->RenderLine(glm::vec2(start.x-100.0f, mCollision.GetEdge(Direction::BOTTOM)), glm::vec2(end.x + 100.0f, mCollision.GetEdge(Direction::BOTTOM)), GREEN);
		//		}

		//		if (mCollision.GetType(Direction::LEFT) == CollisionType::HARD) {
		//			glm::vec2 start(center.x - mAabb.halfWidth, center.y + mAabb.halfHeight - mAabb.margin);
		//			glm::vec2 end(center.x - mAabb.halfWidth, center.y - mAabb.halfHeight + mAabb.margin);

		//			pGraphicsManager->RenderLine(start, end, RED);
		//			pGraphicsManager->RenderLine(glm::vec2(mCollision.GetEdge(Direction::LEFT), start.y - 100.0f), glm::vec2(mCollision.GetEdge(Direction::LEFT), end.y + 100.0f), RED);
		//		}

		//		if (mCollision.GetType(Direction::RIGHT) == CollisionType::HARD) {
		//			glm::vec2 start(center.x + mAabb.halfWidth, center.y + mAabb.halfHeight - mAabb.margin);
		//			glm::vec2 end(center.x + mAabb.halfWidth, center.y - mAabb.halfHeight + mAabb.margin);

		//			pGraphicsManager->RenderLine(start, end, BLUE);
		//			pGraphicsManager->RenderLine(glm::vec2(mCollision.GetEdge(Direction::RIGHT), start.y - 100.0f), glm::vec2(mCollision.GetEdge(Direction::RIGHT), end.y + 100.0f), BLUE);
		//		}
			}
		}
	}
	void PhysicsBody::SetAabbDimensions(float width, float height) {
		mAabb.halfWidth = width / 2.0f; 
		mAabb.halfHeight = height / 2.0f;
	}

	glm::vec2 PhysicsBody::GetAabbCenter()
	{
		glm::vec2 result(0.0f, 0.0f);
		Transform* pTransform = static_cast<Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));
		if (pTransform) {
			result = pTransform->GetPosition() + mAabbOffset;
		}

		return result;
	}

	void PhysicsBody::CheckCollision(PhysicsBody* pOtherBody) {
		if(pOtherBody != this) {
			glm::vec2 pos = GetAabbCenter();
			glm::vec2 otherPos = pOtherBody->GetAabbCenter();

			float left = pos.x - mAabb.halfWidth;
			float right = pos.x + mAabb.halfWidth;
			float top = pos.y + mAabb.halfHeight;
			float bottom = pos.y - mAabb.halfHeight;

			float otherLeft = otherPos.x - pOtherBody->GetAabb().halfWidth;
			float otherRight = otherPos.x + pOtherBody->GetAabb().halfWidth;
			float otherTop = otherPos.y + pOtherBody->GetAabb().halfHeight;
			float otherBottom = otherPos.y - pOtherBody->GetAabb().halfHeight;

			if (!(left > otherRight || otherLeft > right || top < otherBottom || otherTop < bottom)) {
				ObjectCollisionEvent* pEvent = new ObjectCollisionEvent(this, pOtherBody);
				EventManager::GetInstance()->Broadcast(pEvent);
			}
		}
	}

	void PhysicsBody::FallThroughFloor() {
		mSoftCollide.Clear(static_cast<int>(Direction::DOWN));
	}


	void PhysicsBody::HandleEvent(std::string eventName, Event* event) {
		if (event->GetName() == EVT_MAP_COLLISION) {
			mPressingAgainst.Reset();

			MapCollisionEvent* pCollisionEvent = dynamic_cast<MapCollisionEvent*>(event);
			PhysicsBody* pBody = dynamic_cast<PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));
			if (pBody == pCollisionEvent->GetBody()) {
				AABB aabb = pBody->GetAabb();
				glm::vec2 aabbOffset = pBody->GetAabbOffset();

				Transform* pTransform = dynamic_cast<Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));
				glm::vec2 newPosition = pTransform->GetPosition();
				glm::vec2 newSpeed = pTransform->GetSpeed();

			switch (pCollisionEvent->GetDirection()) {
			case Direction::DOWN:
				if (newSpeed.y < 0.0f && (pCollisionEvent->GetType() == CollisionType::HARD || mSoftCollide.Get(static_cast<int>(Direction::DOWN)))) {
					newPosition.y = pCollisionEvent->GetPlane() + aabb.halfHeight - aabbOffset.y;
					newSpeed.y = 0.0f;
					mPressingAgainst.Set(static_cast<int>(Direction::DOWN));
				}
				break;
			case Direction::UP:
				if (newSpeed.y > 0.0f && (pCollisionEvent->GetType() == CollisionType::HARD || mSoftCollide.Get(static_cast<int>(Direction::UP)))) {
					newPosition.y = pCollisionEvent->GetPlane() - aabb.halfHeight - aabbOffset.y;
					newSpeed.y = 0.0f;
					mPressingAgainst.Set(static_cast<int>(Direction::UP));
				}
				break;
			case Direction::LEFT:
				if (newSpeed.x < 0.0f && (pCollisionEvent->GetType() == CollisionType::HARD || mSoftCollide.Get(static_cast<int>(Direction::LEFT)))) {
					newPosition.x = pCollisionEvent->GetPlane() + aabb.halfWidth - aabbOffset.x;
					newSpeed.x = 0.0f;
					mPressingAgainst.Set(static_cast<int>(Direction::LEFT));
				}
				break;
			case Direction::RIGHT:
				if (newSpeed.x > 0.0f && (pCollisionEvent->GetType() == CollisionType::HARD || mSoftCollide.Get(static_cast<int>(Direction::RIGHT)))) {
					newPosition.x = pCollisionEvent->GetPlane() - aabb.halfWidth - aabbOffset.x;
					newSpeed.x = 0.0f;
					mPressingAgainst.Set(static_cast<int>(Direction::RIGHT));
				}
				break;
			}

			if (pCollisionEvent->GetDirection() != Direction::DOWN) {
				mSoftCollide.Set(static_cast<int>(Direction::DOWN));
			}

			pTransform->SetPosition(newPosition.x, newPosition.y);
			pTransform->SetSpeed(newSpeed.x, newSpeed.y);
			}
		}
	}

	bool PhysicsBody::IsAgainstWall(Direction dir) {
		return mPressingAgainst.Get(static_cast<int>(dir));
	}
}