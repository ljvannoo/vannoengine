#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		HealthTracker.h
Purpose:		Simple component for displaying and tracking health
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-28
*************************************************************************/

#define HEALTH_TRACKER_COMPONENT "health_tracker"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"

#include "engine/systems/events/EventHandler.h"

#include <rapidjson/document.h>

// Forward declarations
namespace VannoEngine {
	class GraphicsManager;
	class Event;
}

class HealthTracker : public VannoEngine::GameComponent, public VannoEngine::EventHandler
{
public:
	HealthTracker(VannoEngine::GameObject* owner);
	~HealthTracker() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return HEALTH_TRACKER_COMPONENT;
	}

	bool IsInvulnerable() { return mInvulnerable; }
	bool IsAlive() { return mCurrentHealth > 0.0f; }

	void HandleLocalEvent(std::string eventName, VannoEngine::Event* event);
	void HandleEvent(std::string eventName, VannoEngine::Event* event);

private:
	VannoEngine::GraphicsManager* mpGraphicsManager;

	float mMaxHealth;
	float mCurrentHealth;

	bool mVisible;

	bool mInvulnerable;

	double mDamageCooldown;
};