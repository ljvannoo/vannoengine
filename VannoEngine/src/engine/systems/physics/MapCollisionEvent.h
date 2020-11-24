#pragma once
#include "engine/systems/events/Event.h"

#include "engine/util/Directions.h"

#include <string>

#define EVT_MAP_COLLISION "evt_map_collision"
namespace VannoEngine {
	class PhysicsBody;

	enum class CollisionType {
		NONE = 0,
		HARD = 1,
		SOFT = 2
	};

	class MapCollisionEvent : public Event
	{
	public: // Variables

	public: // Methods
		MapCollisionEvent(PhysicsBody* pBody, std::string layerName, CollisionType type, Direction dir, float plane);
		~MapCollisionEvent();

		PhysicsBody* GetBody() { return mpBody; }
		CollisionType GetType() { return mType; }
		Direction GetDirection() { return mDir; }
		float GetPlane() const { return mPlane; }
		std::string GetLayerName() { return mLayerName; }

	private: // Methods

	private: // Variables
		PhysicsBody* mpBody;
		CollisionType mType;
		Direction mDir;
		float mPlane;
		std::string mLayerName;
	};
}
