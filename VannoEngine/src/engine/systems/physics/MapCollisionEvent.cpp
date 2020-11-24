#include "MapCollisionEvent.h"

namespace VannoEngine {
	MapCollisionEvent::MapCollisionEvent(PhysicsBody* pBody, CollisionType type, Direction dir, float plane) :
		Event(EVT_MAP_COLLISION),
		mpBody{ pBody },
		mType{ type },
		mDir{ dir },
		mPlane{ plane }
	{}

	MapCollisionEvent::~MapCollisionEvent() {

	}
}