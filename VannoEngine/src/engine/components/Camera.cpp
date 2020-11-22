/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Camera.cpp
Purpose:		Handles camera functionality
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-21
*************************************************************************/
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace VannoEngine {
	Camera::Camera(GameObject* owner) :
		GameComponent(owner),
		mScale(100.0f),
		mPosition(glm::vec2(0.0f, 0.0f)),
		mProjectionMatrix(glm::mat4(1.0f)),
		mScreenWidth(800.0f),
		mScreenHeight(600.0f)
	{}

	Camera::~Camera() {

	}


	void Camera::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
		if (pData->HasMember("scale") && (*pData)["scale"].IsFloat()) {
			mScale = (*pData)["scale"].GetFloat();
		}

		if (pData->HasMember("posX") && (*pData)["posX"].IsFloat()) {
			mPosition.x = (*pData)["posX"].GetFloat();
		}

		if (pData->HasMember("posY") && (*pData)["posY"].IsFloat()) {
			mPosition.y = (*pData)["posY"].GetFloat();
		}

		RecalculateProjection();
	}

	void Camera::Update(double deltaTime) {

	}

	void Camera::SetScreenDimensions(float width, float height) {
		mScreenWidth = width;
		mScreenHeight = height;
		RecalculateProjection();
	}

	void Camera::SetPosition(float x, float y) {
		mPosition.x = x;
		mPosition.y = y;
		RecalculateProjection();
	}

	void Camera::SetPosition(glm::vec2 const& position) {
		mPosition.x = position.x;
		mPosition.y = position.y;
		RecalculateProjection();
	}

	void Camera::SetScale(float s) {
		mScale = s;
		RecalculateProjection();
	}

	void Camera::RecalculateProjection() {
		mProjectionMatrix = glm::ortho(-mScreenWidth / 2.0f, mScreenWidth / 2.0f, -mScreenHeight / 2.0f, mScreenHeight / 2.0f);
		mProjectionMatrix = glm::scale(mProjectionMatrix, glm::vec3(mScale, mScale, 1.0f));
		mProjectionMatrix = glm::translate(mProjectionMatrix, glm::vec3(-mPosition.x, -mPosition.y, 0.0f));
	}

	bool Camera::InView(glm::vec2 const& min, glm::vec2 const& max) {
		float hW = GetScaledWidth() / 2.0f;
		float hH = GetScaledHeight() / 2.0f;
		
		if (min.x < mPosition.x - hW || max.x > mPosition.x + hW) {
			return false;
		}

		if (min.y < mPosition.y - hH || max.y > mPosition.y + hH) {
			return false;
		}
		return true;
	}

	bool Camera::InViewX(float x) {
		float hW = GetScaledWidth() / 2.0f;
		float hH = GetScaledHeight() / 2.0f;

		if (x < mPosition.x - hW || x > mPosition.x + hW) {
			return false;
		}

		return true;
	}
}