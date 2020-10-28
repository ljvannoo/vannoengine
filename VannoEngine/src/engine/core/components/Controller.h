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

#include "engine/core/components/GameComponent.h"
#include "engine/systems/EventHandler.h"

namespace VannoEngine {
	class GameObject;

	class Controller : public GameComponent, public EventHandler
	{
	public:
		Controller(GameObject* owner) : GameComponent(owner) {}
		~Controller() override;

		void Update(double deltaTime);

		void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

		std::string GetType() override {
			return CONTROLLER_COMPONENT;
		}

		void HandleEvent(std::string eventName, std::string data) override;
	};
}