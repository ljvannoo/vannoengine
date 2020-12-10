#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		MainMenuController.h
Purpose:		Used to define behavior of the main menu
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Dec-05
*************************************************************************/

#define MAINMENU_COMPONENT "main_menu_controller"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"

#include <rapidjson/document.h>

namespace VannoEngine {
	class InputManager;
}
class MainMenuController : public VannoEngine::GameComponent
{
public:
	enum class State : int {
		PlayGame,
		ViewControls,
		ShowingControls,
		Credits,
		ShowingCredits,
		QuitGame
	};

	MainMenuController(VannoEngine::GameObject* owner);
	~MainMenuController() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return MAINMENU_COMPONENT;
	}

private:
	VannoEngine::InputManager* mpInputManager;
	State mCurrentState;

	std::string mFirstLevelFilename;
};