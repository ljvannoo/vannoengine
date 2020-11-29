#pragma once
#include "engine/systems/events/Event.h"

#define EVT_INVULNERABLE "evt_invulnerable"

namespace VannoEngine {
	class GameObject;
}
class InvulnerableEvent : public VannoEngine::Event
{
public: // Methods
	InvulnerableEvent(VannoEngine::GameObject* pObj, bool state) :
		Event(EVT_INVULNERABLE),
		mpObj{ pObj },
		mState{ state }
	{ }
	~InvulnerableEvent() {}

	VannoEngine::GameObject* GetObj() { return mpObj; }
	bool GetState() { return mState; }
private: // Variables
	VannoEngine::GameObject* mpObj;
	bool mState;
};
