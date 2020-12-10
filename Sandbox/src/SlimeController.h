#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		SlimeController.h
Purpose:		Used to define behavior of the slime characters
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-28
*************************************************************************/

#define SLIME_CONTROLLER_COMPONENT "slime_controller"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"
#include "engine/systems/events/EventHandler.h"

#include <rapidjson/document.h>

class SlimeController : public VannoEngine::GameComponent, public VannoEngine::EventHandler
{
private:
	enum class State : int {
		Idle,
		Walk,
		Dieing,
		Dead
	};
public:
	SlimeController(VannoEngine::GameObject* owner);
	~SlimeController() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	void HandleEvent(std::string eventName, VannoEngine::Event* event);
	void HandleLocalEvent(std::string eventName, VannoEngine::Event* event);

	std::string GetType() override {
		return SLIME_CONTROLLER_COMPONENT;
	}

	void TurnAround();
private:
	float MoveTowards(float current, float target, float maxDelta);
	void SpawnMinis();
private:
	const float cWalkSpeed = 1.0f;
	const float cWalkAccel = 60.0f;
	const double cDamageCooldown = 1.0;

	float mDirection = 1.0f;

	double mCooldown;

	State mCurrentState;
};