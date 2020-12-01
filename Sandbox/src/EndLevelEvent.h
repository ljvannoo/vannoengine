#pragma once
#include "engine/systems/events/Event.h"

#define EVT_END_LEVEL "evt_end_level"

namespace VannoEngine {
	class GameObject;
}
class EndLevelEvent : public VannoEngine::Event
{
public: // Methods
	EndLevelEvent() :
		Event(EVT_END_LEVEL)
	{ }
	~EndLevelEvent() {}
private: // Variables
};
