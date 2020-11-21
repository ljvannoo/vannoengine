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
	Stand		= 0,
	Stand2		= 1,
	Slowing		= 2,
	Crouch		= 3,
	Crouch2		= 4,
	Walk		= 5,
	Jump		= 6,
	Falling		= 7,
	Slide		= 8
};

namespace VannoEngine {
	class Transform;
	class PhysicsBody;
	class InputManager;
	class PhysicsManager;
	class ConfigurationManager;
	class LevelManager;
}
class Controller : public VannoEngine::GameComponent
{
public:
	Controller(VannoEngine::GameObject* owner);
	~Controller() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return CONTROLLER_COMPONENT;
	}
private:
	void HandleCollisions(VannoEngine::Transform* pTransform, VannoEngine::PhysicsBody* pBody);
	void UpdateCamera(VannoEngine::Transform* pTransform);
	void Controller::Jump();

private:
	VannoEngine::InputManager* mpInputManager;
	VannoEngine::PhysicsManager* mpPhysicsManager;
	VannoEngine::ConfigurationManager* mpConfigManager;
	VannoEngine::LevelManager* mpLevelManager;

	State mCurrentState;
	const float cWalkSpeed = 300.0f;
	const float cJumpSpeed = 400.0f;
	const float cMinJumpSpeed = 200.0f;
	const float cWalkAccel = 0.8f;
};