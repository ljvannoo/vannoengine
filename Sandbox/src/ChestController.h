#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		ChestController.h
Purpose:		Defines the behavior of treasure chests
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Dec-10
*************************************************************************/
#define CHEST_CONTROLLER_COMPONENT "chest_controller"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"

#include <rapidjson/document.h>

class ChestController : public VannoEngine::GameComponent
{
public:
	enum class State : int {
		Closed,
		Opening,
		Open
	};

	ChestController(VannoEngine::GameObject* owner);
	~ChestController() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return CHEST_CONTROLLER_COMPONENT;
	}

	void HandleLocalEvent(std::string eventName, VannoEngine::Event* event) override;

private:
	std::string mContents;

	State mCurrentState;
};