#pragma once
#include "engine/systems/events/Event.h"

#define EVT_DESTROY_OBJECT "evt_destroy_object"
namespace VannoEngine {
	class GameObject;

	class DestroyObjectEvent : public Event
	{
	public: // Methods
		DestroyObjectEvent(GameObject* pObject) :
			Event(EVT_DESTROY_OBJECT),
			mpObject{ pObject }
			
		{ }
		~DestroyObjectEvent() {}

		GameObject* GetObj() { return mpObject; }
	private: // Variables
		GameObject* mpObject;
	};
}
