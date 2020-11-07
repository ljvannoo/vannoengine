/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Camera.h
Purpose:		Handles camera functionality
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-21
*************************************************************************/
#pragma once

#include "GameComponent.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <string>

#define CAMERA_COMPONENT "camera"


namespace VannoEngine {
	class Camera : public GameComponent
	{
	public: // Variables

	public: // Methods
		Camera(GameObject* owner);
		~Camera() override;

		std::string GetType() override { return CAMERA_COMPONENT; }

		void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

		void Update(double deltaTime) override;

		void SetPosition(float x, float y);
		void SetPosition(glm::vec2 const& position);
		void SetScale(float s);

		void SetScreenDimensions(float width, float height);
		float GetScreenWidth() { return mScreenWidth; }
		float GetScreenHeight() { return mScreenHeight; }

		glm::mat4 GetProjectionMatrix() { return mProjectionMatrix; }

	private: // Methods
		void RecalculateProjection();

	private: // Variables
		glm::vec2 mPosition;
		float mScale;
		float mScreenWidth;
		float mScreenHeight;
		glm::mat4 mProjectionMatrix;
	};
}