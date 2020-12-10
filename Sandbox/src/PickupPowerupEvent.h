#pragma once
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
