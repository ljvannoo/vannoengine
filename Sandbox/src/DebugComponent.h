#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		DebugComponent.h
Purpose:		Simple component for debugging. Should not be used.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-27
*************************************************************************/

#define DEBUG_COMPONENT "debug"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"

#include <rapidjson/document.h>

class DebugComponent : public VannoEngine::GameComponent
{
public:
	DebugComponent(VannoEngine::GameObject* owner);
	~DebugComponent() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return DEBUG_COMPONENT;
	}

private:
};