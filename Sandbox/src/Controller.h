#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Controller.h
Purpose:		Component for handling input
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-19
*************************************************************************/
#pragma once

#define CONTROLLER_COMPONENT "controller"

#include <rapidjson/document.h>

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"

enum class State {
	Stand,
	Walk,
	Jump
};

namespace VannoEngine {
	class Transform;
	class PhysicsBody;
}
class Controller : public VannoEngine::GameComponent
{
public:
	Controller(VannoEngine::GameObject* owner);
	~Controller() override;

	void Update(double deltaTime);

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return CONTROLLER_COMPONENT;
	}
private:
	void HandleCollisions(VannoEngine::Transform* pTransform, VannoEngine::PhysicsBody* pBody);
private:
	State mCurrentState;
	const float cWalkSpeed = 300.0f;
	const float cJumpSpeed = 800.0f;
	const float cMinJumpSpeed = 200.0f;
};