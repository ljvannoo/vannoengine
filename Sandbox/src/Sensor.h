#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Sensor.h
Purpose:		Used as a child object to check for opponents within range
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-29
*************************************************************************/

#define SENSOR_COMPONENT "sensor"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"

#include "engine/systems/events/EventHandler.h"

#include <rapidjson/document.h>

// Forward declarations
namespace VannoEngine {
	class GraphicsManager;
	class Event;
}

class Sensor : public VannoEngine::GameComponent, public VannoEngine::EventHandler
{
public:
	Sensor(VannoEngine::GameObject* owner);
	~Sensor() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return SENSOR_COMPONENT;
	}

	void HandleEvent(std::string eventName, VannoEngine::Event* event);
	void HandleLocalEvent(std::string eventName, VannoEngine::Event* event);

private:

};