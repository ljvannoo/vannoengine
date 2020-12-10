#pragma once
#include "engine/systems/events/Event.h"

#define EVT_UNLOCK_EXIT "evt_unlock_exit"

class UnlockExitEvent : public VannoEngine::Event
{
public: // Methods
	UnlockExitEvent() :
		Event(EVT_UNLOCK_EXIT)
	{ }
	~UnlockExitEvent() {}
private: // Variables
};
