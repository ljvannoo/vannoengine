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
	class PhysicsBody : public GameComponent
	{
	public: // Variables

	public: // Methods
		PhysicsBody(GameObject* owner);
		~PhysicsBody() override;

		std::string GetType() override { return PHYSICSBODY_COMPONENT; }
		void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData);

		void Update(double deltaTime);

		void SetDimensions(float width, float height) { mDimensions = glm::vec2(width, height); }
		float GetWidth() { return mDimensions.x; }
		float GetHeight() { return mDimensions.y; }

	private: // Methods

	private: // Variables
		glm::vec2 mDimensions;
	};
}
