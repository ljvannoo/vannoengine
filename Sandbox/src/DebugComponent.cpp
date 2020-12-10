/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		DebugComponent.cpp
Purpose:		Simple component for debugging. Should not be used.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-27
*************************************************************************/

#include "DebugComponent.h"

#include "engine/core/Log.h"

DebugComponent::DebugComponent(VannoEngine::GameObject* owner) :
	GameComponent(owner)
{ }

DebugComponent::~DebugComponent()
{ }


void DebugComponent::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
}

void DebugComponent::Update(double deltaTime) {
	LOG_DEBUG("Here!");
}

void DebugComponent::Draw() {

}

