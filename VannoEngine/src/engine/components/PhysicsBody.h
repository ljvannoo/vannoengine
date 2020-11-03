#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		PhysicsBody.h
Purpose:		Component to describe a physics body
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-01
*************************************************************************/
#define PHYSICSBODY_COMPONENT "physicsbody"

#include "GameComponent.h"

#include <glm/vec2.hpp>

#include <string>

namespace VannoEngine {
	class AABB {
	public:
		glm::vec2 center;
		float halfWidth;
		float halfHeight;

		AABB();
		AABB(float x, float y, float width, float height);

		~AABB() {};

		bool Collides(AABB const& other);
	};

	class PhysicsBody : public GameComponent
	{
	public: // Variables

	public: // Methods
		PhysicsBody(GameObject* owner);
		~PhysicsBody() override;

		std::string GetType() override { return PHYSICSBODY_COMPONENT; }
		void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData);

		void Update(double deltaTime);

		void SetWidth(float width) { mAabb.halfWidth = width / 2.0f; }
		void SetHeight(float height) { mAabb.halfHeight = height / 2.0f; }

		bool IsOnGround() { return mOnGround; }
		void SetOnGround(bool state) { mOnGround = state; }

	private: // Methods

	private: // Variables
		AABB mAabb;
		glm::vec2 mAabbOffset;
		
		glm::vec2 mOldPosition;

		glm::vec2 mOldSpeed;

		bool mWasOnGround;
		bool mOnGround;
	};
}
