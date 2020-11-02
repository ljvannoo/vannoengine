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

#include "Transform.h"

#include "engine/systems/objects/GameObject.h"

#include <math.h>
namespace VannoEngine {
	AABB::AABB() :
		center(glm::vec2(0.0f, 0.0f)),
		halfWidth(0.0f),
		halfHeight(0.0f)
	{}

	AABB::AABB(float x, float y, float width, float height) :
		center(glm::vec2(x, y)),
		halfWidth(width / 2.0f),
		halfHeight(height / 2.0f)
	{}

	bool AABB::Collides(AABB const& other) {
		if (fabsf(center.x - other.center.x) > halfWidth + other.halfWidth) {
			return false;
		}
		if (fabsf(center.y - other.center.y) > halfHeight + other.halfHeight) {
			return false;
		}

		return true;
	}

	PhysicsBody::PhysicsBody(GameObject* owner) : GameComponent(owner) {

	}

	PhysicsBody::~PhysicsBody() {

	}
	void PhysicsBody::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
	}

	void PhysicsBody::Update(double deltaTime) {
		GameObject* pObject = GetOwner();
		Transform* pTransform = static_cast<Transform*>(pObject->GetComponent(TRANSFORM_COMPONENT));

		if (pTransform) {
			// Update old values
			glm::vec2 position = pTransform->GetPosition();
			mOldPosition = position;
			glm::vec2 speed = pTransform->GetSpeed();
			mOldSpeed = speed;

			mWasOnGround = mOnGround;

			// Update position
			position += speed * (float)deltaTime;

			// Check to see if on the ground
			if (position.y < 0.0f) {
				position.y == 0.0f;
				mOnGround = true;
			}
			else {
				mOnGround = false;
			}

			// Update AABB
			mAabb.center = position + mAabbOffset;

			pTransform->SetPosition(position.x, position.y);
		}
	}
}