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

#include "engine/systems/events/EventHandler.h"

#include "engine/systems/physics/Aabb.h"

#include "engine/util/Bits.h"

#include <glm/vec2.hpp>

#include <string>


namespace VannoEngine {
	class Event;

	class PhysicsBody : public GameComponent, public EventHandler
	{
	public: // Variables

	public: // Methods
		PhysicsBody(GameObject* owner);
		~PhysicsBody() override;

		std::string GetType() override { return PHYSICSBODY_COMPONENT; }
		void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData);

		void Update(double deltaTime);
		void Draw() override;

		void HandleEvent(std::string eventName, VannoEngine::Event* event) override;

		AABB const& GetAabb() { return mAabb; }

		void SetAabbOffset(float x, float y) { mAabbOffset = glm::vec2(x, y); }
		glm::vec2 const& GetAabbOffset() { return mAabbOffset; }

		void SetAabbDimensions(float width, float height);
		glm::vec2 GetAabbHalfDimensions() { return glm::vec2(mAabb.halfWidth, mAabb.halfHeight); }
		glm::vec2 GetAabbCenter();

		void CheckCollision(PhysicsBody* pOtherBody);

		void FallThroughFloor();
	private: // Methods

	private: // Variables
		AABB mAabb;
		glm::vec2 mAabbOffset;
		float mMass;

		Bits<4> mPressingAgainst;
		Bits<4> mSoftCollide;
		
	};
}
