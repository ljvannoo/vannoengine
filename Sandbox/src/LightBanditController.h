#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		LightBandit.h
Purpose:		Used to define behavior of the Light Bandit characters
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-29
*************************************************************************/
#define LIGHT_BANDIT_CONTROLLER_COMPONENT "light_bandit_controller"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"
#include "engine/systems/events/EventHandler.h"

#include <rapidjson/document.h>

class LightBanditController : public VannoEngine::GameComponent, public VannoEngine::EventHandler
{
private:
	enum class State : int {
		Idle,
		Alert,
		Run,
		Attack,
		Dieing,
		Dead
	};
public:
	LightBanditController(VannoEngine::GameObject* owner);
	~LightBanditController() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	void HandleEvent(std::string eventName, VannoEngine::Event* event);
	void HandleLocalEvent(std::string eventName, VannoEngine::Event* event);

	std::string GetType() override {
		return LIGHT_BANDIT_CONTROLLER_COMPONENT;
	}

	void TurnAround();
private:
	float MoveTowards(float current, float target, float maxDelta);
private:
	const float cWalkSpeed = 2.0f;
	const float cWalkAccel = 60.0f;
	const double cDamageCooldown = 1.0;
	const double cPatrolCooldown = 3.0;
	const double cAttackDuration = 1.6;
	float mDirection;

	double mCooldown;

	double mAttackCooldown;

	const double cAlertCooldown = 5.0;
	double mAlertCooldown;

	bool mCanDoDamage;

	float mDamage;


	State mCurrentState;
};