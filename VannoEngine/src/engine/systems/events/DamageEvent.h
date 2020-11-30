#pragma once
#include "engine/systems/events/Event.h"

#define EVT_DAMAGE "evt_damage"

namespace VannoEngine {
	class GameObject;

	class DamageEvent : public VannoEngine::Event
	{
	public: // Methods
		DamageEvent(VannoEngine::GameObject* pSource, VannoEngine::GameObject* pTarget, float amount) :
			Event(EVT_DAMAGE),
			mpSource{ pSource },
			mpTarget{ pTarget },
			mAmount{ amount }
		{ }
		~DamageEvent() {}

		VannoEngine::GameObject* GetSource() { return mpSource; }
		VannoEngine::GameObject* GetTarget() { return mpTarget; }
		float GetAmount() { return mAmount; }

	private: // Variables
		VannoEngine::GameObject* mpSource;
		VannoEngine::GameObject* mpTarget;
		float mAmount;
	};
}