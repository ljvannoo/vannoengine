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

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"

#include <rapidjson/document.h>

enum class State : int {
	Stand,
	Crouch,
	Walk,
	Run,
	Dash,
	Jumping,
	Jump,
	Fall,
	Attack
};

namespace VannoEngine {
	class Transform;
	class PhysicsBody;
	class InputManager;
	class PhysicsManager;
	class ConfigurationManager;
	class LevelManager;
	class TimeManager;
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
	void Jump();
	float MoveTowards(float current, float target, float maxDelta);

private:
	VannoEngine::InputManager* mpInputManager;
	VannoEngine::PhysicsManager* mpPhysicsManager;
	VannoEngine::ConfigurationManager* mpConfigManager;
	VannoEngine::LevelManager* mpLevelManager;
	VannoEngine::TimeManager* mpTimeManager;

	const float cWalkSpeed = 2.0f;
	const float cWalkAccel = 60.0f;
	const float cRunSpeed = 4.0f;
	const float cDashSpeed = 6.0f;
	const float cJumpHeight = 1.2f;
	const unsigned long cAttackDuration = 400l;

	State mCurrentState;
	unsigned long mAttackStartTime;
	unsigned short mAttackNum = 1;
};