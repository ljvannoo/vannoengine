/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Transform.cpp
Purpose:		Implementation of the Transform component
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-16
*************************************************************************/
#include "Transform.h"

#include "engine/systems/graphics/GraphicsManager.h"

#include "engine/systems/objects/GameObject.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace VannoEngine {
	Transform::Transform(GameObject* owner) :
		GameComponent(owner),
		mScale(glm::vec2(1.0f, 1.0f)),
		mPosition(glm::vec2(0.0f, 0.0f)),
		mSpeed(0.0f),
		mRotation(0.0f)
	{ }

	Transform::~Transform() {
	}

	void Transform::Update(double deltaTime) {

	}

	void Transform::Draw()
	{
		/*
		GraphicsManager* pGraphicsManager = GraphicsManager::GetInstance();

		pGraphicsManager->RenderCircle(mPosition, 2.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), true);
		*/
	}

	void Transform::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
		if (pData->HasMember("scaleX") && (*pData)["scaleX"].IsFloat()) {
			mScale.x = (*pData)["scaleX"].GetFloat();
		}

		if (pData->HasMember("scaleY") && (*pData)["scaleY"].IsFloat()) {
			mScale.y = (*pData)["scaleY"].GetFloat();
		}

		if (pData->HasMember("rotation") && (*pData)["rotation"].IsFloat()) {
			mRotation = (*pData)["rotation"].GetFloat();
		}

		if (pData->HasMember("posX") && (*pData)["posX"].IsFloat()) {
			mPosition.x = (*pData)["posX"].GetFloat();
		}

		if (pData->HasMember("posY") && (*pData)["posY"].IsFloat()) {
			mPosition.y = (*pData)["posY"].GetFloat();
		}

		if (pData->HasMember("speedX") && (*pData)["speedX"].IsFloat()) {
			mSpeed.x = (*pData)["speedX"].GetFloat();
		}

		if (pData->HasMember("speedY") && (*pData)["speedY"].IsFloat()) {
			mSpeed.y = (*pData)["speedY"].GetFloat();
		}
	}


	glm::mat4 Transform::GetTranslationMatrix() {
		glm::mat4 result = glm::mat4(1.0f);

		GameObject* pParentObject = GetOwner()->GetParentObject();
		if (pParentObject) {
			Transform* pParentTransform = static_cast<Transform*>(pParentObject->GetComponent(TRANSFORM_COMPONENT));
			if (pParentTransform) {
				result = pParentTransform->GetTranslationMatrix();
			}
		}
		return glm::translate(result, glm::vec3(GetPosition().x, GetPosition().y, 0.0f));
	}

	glm::mat4 Transform::GetRotationMatrix() {
		glm::mat4 result = glm::mat4(1.0f);

		GameObject* pParentObject = GetOwner()->GetParentObject();
		if (pParentObject) {
			Transform* pParentTransform = static_cast<Transform*>(pParentObject->GetComponent(TRANSFORM_COMPONENT));
			if (pParentTransform) {
				result = pParentTransform->GetRotationMatrix();
			}
		}
		return glm::rotate(result, glm::radians(-GetRotation()), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	glm::mat4 Transform::GetScaleMatrix() {
		glm::mat4 result = glm::mat4(1.0f);

		GameObject* pParentObject = GetOwner()->GetParentObject();
		if (pParentObject) {
			Transform* pParentTransform = static_cast<Transform*>(pParentObject->GetComponent(TRANSFORM_COMPONENT));
			if (pParentTransform) {
				result = pParentTransform->GetScaleMatrix();
			}
		}
		return glm::scale(result, glm::vec3(GetScale().x, GetScale().y, 1.0f));
	}

	glm::vec2 Transform::GetPosition() {

		GameObject* pParent = GetOwner()->GetParentObject();
		if (pParent && pParent->HasComponent(TRANSFORM_COMPONENT)) {
			Transform* pParentTransform = dynamic_cast<Transform*>(pParent->GetComponent(TRANSFORM_COMPONENT));
			glm::vec2 parentPos = pParentTransform->GetPosition();
			return mPosition + parentPos;
		}

		return mPosition;
	}

	void Transform::SetPosition(float x, float y) { 
		GameObject* pParent = GetOwner()->GetParentObject();
		if (pParent && pParent->HasComponent(TRANSFORM_COMPONENT)) {
			Transform* pParentTransform = dynamic_cast<Transform*>(pParent->GetComponent(TRANSFORM_COMPONENT));
			glm::vec2 parentPos = pParentTransform->GetPosition();
			mPosition.x = x - parentPos.x;
			mPosition.y = y - parentPos.y;
		}
		else {
			mPosition = glm::vec2(x, y);
		}
	}

	void Transform::SetPositionX(float x) { 
		GameObject* pParent = GetOwner()->GetParentObject();
		if (pParent && pParent->HasComponent(TRANSFORM_COMPONENT)) {
			Transform* pParentTransform = dynamic_cast<Transform*>(pParent->GetComponent(TRANSFORM_COMPONENT));
			glm::vec2 parentPos = pParentTransform->GetPosition();
			mPosition.x = x - parentPos.x;
		}
		else {
			mPosition.x = x;
		}
	}

	void Transform::SetPositionY(float y) {
		GameObject* pParent = GetOwner()->GetParentObject();
		if (pParent && pParent->HasComponent(TRANSFORM_COMPONENT)) {
			Transform* pParentTransform = dynamic_cast<Transform*>(pParent->GetComponent(TRANSFORM_COMPONENT));
			glm::vec2 parentPos = pParentTransform->GetPosition();
			mPosition.y = y - parentPos.y;
		}
		else {
			mPosition.y = y;
		}
	}
}