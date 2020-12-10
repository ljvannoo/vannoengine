#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		PickupPowerupEvent.h
Purpose:		Used to signal that the player has picked up a powerup
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Dec-10
*************************************************************************/

#include "engine/systems/events/Event.h"

#define EVT_PICKUP_POWERUP "evt_pickup_powerup"

namespace VannoEngine {
	class GameObject;
}

class PowerupPickupEvent : public VannoEngine::Event
{
public: // Methods
	PowerupPickupEvent(VannoEngine::GameObject* pSource, VannoEngine::GameObject* pPowerup) :
		Event(EVT_PICKUP_POWERUP),
		mpSource{ pSource },
		mpPowerup{ pPowerup }
	{ }
	~PowerupPickupEvent() {}

	VannoEngine::GameObject* GetSource() { return mpSource; }
	VannoEngine::GameObject* GetPowerup() { return mpPowerup; }
private: // Variables
	VannoEngine::GameObject* mpSource;
	VannoEngine::GameObject* mpPowerup;
};
