/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Transform.h
Purpose:		Component for maintaining transform information
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-16
*************************************************************************/
#pragma once

#define TRANSFORM_COMPONENT "transform"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <rapidjson/document.h>

#include "engine/components/GameComponent.h"


namespace VannoEngine {
	class Transform : public GameComponent
	{
	public:
		Transform(GameObject* owner);

		~Transform() override;

		void Update(double deltaTime);

		void Draw() override;

		void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

		std::string GetType() override {
			return TRANSFORM_COMPONENT;
		}

		glm::vec2 GetPosition();
		glm::vec2 GetScale() const { return mScale; }
		float GetRotation() const { return mRotation; }

		void SetPosition(float x, float y);
		void SetPositionX(float x);
		void SetPositionY(float y);
		void SetScale(float x, float y) { mScale = glm::vec2(x, y); }

		void SetSpeed(float x, float y) { mSpeed = glm::vec2(x, y); }
		void SetSpeedX(float x) { mSpeed.x = x; }
		void SetSpeedY(float y) { mSpeed.y = y; }
		void IncreaseSpeed(float x, float y) { mSpeed.x += x; mSpeed.y += y; }
		glm::vec2 GetSpeed() { return mSpeed; }

		void SetRotation(float angleDeg) { mRotation = angleDeg; }

		glm::mat4 GetTranslationMatrix();
		glm::mat4 GetRotationMatrix();
		glm::mat4 GetScaleMatrix();

	private:
		glm::vec2 mScale;
		glm::vec2 mPosition;
		glm::vec2 mSpeed;
		float mRotation;
	};
}