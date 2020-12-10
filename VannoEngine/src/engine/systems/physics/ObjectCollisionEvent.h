#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		ObjectCollisionEvent.h
Purpose:		Signals that an object has collided with another object
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-24
*************************************************************************/
#include "engine/systems/events/Event.h"

#define EVT_OBJECT_COLLISION "evt_obj_collision"
namespace VannoEngine {
	class PhysicsBody;

	class ObjectCollisionEvent : public Event
	{
	public: // Methods
		ObjectCollisionEvent(PhysicsBody* pBody, PhysicsBody* pOtherBody) :
			Event(EVT_OBJECT_COLLISION),
			mpBody{ pBody },
			mpOtherBody{ pOtherBody }
		{ }
		~ObjectCollisionEvent() {}

		PhysicsBody* GetBody() { return mpBody; }
		PhysicsBody* GetOtherBody() { return mpOtherBody; }
	private: // Variables
		PhysicsBody* mpBody;
		PhysicsBody* mpOtherBody;
	};
}
