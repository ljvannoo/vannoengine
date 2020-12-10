#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		PowerUp.h
Purpose:		Used to define behavior of powerups on the level
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-24
*************************************************************************/

#define POWERUP_COMPONENT "powerup"

#include "engine/components/GameComponent.h"

// Forward declarations
namespace VannoEngine {
	class GameObject;
}

class PowerUp : public VannoEngine::GameComponent
{
public: // Variables

public: // Methods
	PowerUp(VannoEngine::GameObject* owner);
	~PowerUp();

	std::string GetType() override { return POWERUP_COMPONENT; }
	
	void Update(double deltaTime) override;

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

private: // Methods

private: // Variables
};
