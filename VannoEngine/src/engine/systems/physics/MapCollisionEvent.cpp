#include "MapCollisionEvent.h"

namespace VannoEngine {
	MapCollisionEvent::MapCollisionEvent(PhysicsBody* pBody, std::string layerName, CollisionType type, Direction dir, float plane) :
		Event(EVT_MAP_COLLISION),
		mpBody{ pBody },
		mLayerName{ layerName },
		mType{ type },
		mDir{ dir },
		mPlane{ plane }
	{}

	MapCollisionEvent::~MapCollisionEvent() {

	}
}