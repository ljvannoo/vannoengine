#include "ObjectCollisionEvent.h"

namespace VannoEngine {
	ObjectCollisionEvent::ObjectCollisionEvent(PhysicsBody* pBody, PhysicsBody* pOtherBody) :
		Event(EVT_OBJECT_COLLISION),
		mpBody{ pBody },
		mpOtherBody{ pOtherBody }
	{ }

	ObjectCollisionEvent::~ObjectCollisionEvent() {

	}
}