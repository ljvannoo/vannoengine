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

#define CONTROLLER_COMPONENT "controller"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"
#include "engine/systems/events/EventHandler.h"

#include <rapidjson/document.h>

enum class State : int {
	Stand,	// 0
	Crouch,	// 1
	Walk,	// 2
	Run,	// 3
	Dash,	// 4
	Jumping,// 5
	Jump,	// 6
	Fall,	// 7
	Hurt,	// 8
	Attack,	// 9
	Shoot,	// 10
	Dieing,	// 11
	Dead,	// 12
	GameOver
};

namespace VannoEngine {
	class Transform;
	class PhysicsBody;
	class InputManager;
	class PhysicsManager;
	class ConfigurationManager;
	class LevelManager;
	class TimeManager;
	class Event;
}
class Controller : public VannoEngine::GameComponent, public VannoEngine::EventHandler
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

	void HandleLocalEvent(std::string eventName, VannoEngine::Event* event);
	void HandleEvent(std::string eventName, VannoEngine::Event* event) override;

private:
	//void HandleCollisions(VannoEngine::Transform* pTransform, VannoEngine::PhysicsBody* pBody);
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
	unsigned long mAttackDuration = 200l;

	State mCurrentState;

	unsigned long mAttackStartTime;
	bool mHasSword;
	bool mHasBow;
	bool mCanDoDamage;

	float mFistDamage;
	float mSwordDamage;

	double mCooldown;
};