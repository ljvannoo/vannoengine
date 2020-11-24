#pragma once
#include "engine/systems/events/Event.h"

#define EVT_OBJECT_COLLISION "evt_obj_collision"
namespace VannoEngine {
	class PhysicsBody;

	class ObjectCollisionEvent : public Event
	{
	public: // Methods
		ObjectCollisionEvent(PhysicsBody* pBody, PhysicsBody* pOtherBody);
		~ObjectCollisionEvent();

		PhysicsBody* GetBody() { return mpBody; }
		PhysicsBody* GetOtherBody() { return mpOtherBody; }
	private: // Variables
		PhysicsBody* mpBody;
		PhysicsBody* mpOtherBody;
	};
}
