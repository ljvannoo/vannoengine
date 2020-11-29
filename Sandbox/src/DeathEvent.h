#pragma once
#include "engine/systems/events/Event.h"

#define EVT_DEATH "evt_death"

namespace VannoEngine {
	class GameObject;
}
class DeathEvent : public VannoEngine::Event
{
public: // Methods
	DeathEvent(VannoEngine::GameObject* pObj) :
		Event(EVT_DEATH),
		mpObj{ pObj }
	{ }
	~DeathEvent() {}

	VannoEngine::GameObject* GetObj() { return mpObj; }

private: // Variables
	VannoEngine::GameObject* mpObj;
};
