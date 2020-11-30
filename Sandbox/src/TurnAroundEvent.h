#pragma once
#include "engine/systems/events/Event.h"

#define EVT_TURN_AROUND "evt_turn_around"

namespace VannoEngine {
	class GameObject;
}
class TurnAroundEvent : public VannoEngine::Event
{
public: // Methods
	TurnAroundEvent(VannoEngine::GameObject* pObj) :
		Event(EVT_TURN_AROUND),
		mpObj{ pObj }
	{ }
	~TurnAroundEvent() {}

	VannoEngine::GameObject* GetObj() { return mpObj; }

private: // Variables
	VannoEngine::GameObject* mpObj;
};
