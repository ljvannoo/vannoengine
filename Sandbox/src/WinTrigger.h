#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		WinTrigger.h
Purpose:		Used to define behavior of the object that triggers the
				win state when a player passes through it.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Dec-09
*************************************************************************/

#define WINTRIGGER_COMPONENT "win_trigger"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"
#include "engine/systems/events/EventHandler.h"

#include <rapidjson/document.h>

class WinTrigger : public VannoEngine::GameComponent, public VannoEngine::EventHandler
{
public:
	WinTrigger(VannoEngine::GameObject* owner);
	~WinTrigger() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return WINTRIGGER_COMPONENT;
	}

	void HandleEvent(std::string eventName, VannoEngine::Event* event) override;

private:
	bool mUnlocked;
};