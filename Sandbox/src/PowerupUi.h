#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		PowerupUi.h
Purpose:		Used to define behavior of the powerup UI objects
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Dec-10
*************************************************************************/

#define POWERUP_UI_COMPONENT "powerup_ui"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"
#include "engine/systems/events/EventHandler.h"

#include <rapidjson/document.h>

class PowerupUi : public VannoEngine::GameComponent, public VannoEngine::EventHandler
{
public:
	PowerupUi(VannoEngine::GameObject* owner);
	~PowerupUi() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return POWERUP_UI_COMPONENT;
	}

	void HandleEvent(std::string eventName, VannoEngine::Event* event) override;
private:
	std::string mType;
};