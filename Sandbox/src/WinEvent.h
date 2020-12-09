#pragma once
#include "engine/systems/events/Event.h"

#define EVT_WIN "evt_win"

class WinEvent : public VannoEngine::Event
{
public: // Methods
	WinEvent() :
		Event(EVT_WIN)
	{ }
	~WinEvent() {}
private: // Variables
};
