#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		MapCollisionEvent.h
Purpose:		Signals that an object has collided with the map
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-23
*************************************************************************/

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
		MapCollisionEvent(PhysicsBody* pBody, std::string layerName, CollisionType type, Direction dir, float plane) :
			Event(EVT_MAP_COLLISION),
			mpBody{ pBody },
			mLayerName{ layerName },
			mType{ type },
			mDir{ dir },
			mPlane{ plane }
		{}
		~MapCollisionEvent() {}

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
