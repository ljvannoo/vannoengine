#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		ArrowController.h
Purpose:		Defines the behavior of arrows fired by the player
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-27
*************************************************************************/
#define ARROW_CONTROLLER_COMPONENT "arrow_controller"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"
#include "engine/systems/events/EventHandler.h"

#include <rapidjson/document.h>

class ArrowController : public VannoEngine::GameComponent, public VannoEngine::EventHandler
{
public:
	ArrowController(VannoEngine::GameObject* owner);
	~ArrowController() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	void HandleEvent(std::string eventName, VannoEngine::Event* event);

	std::string GetType() override {
		return ARROW_CONTROLLER_COMPONENT;
	}

private:
	double mTimeToLive;
	float mDamage;
};