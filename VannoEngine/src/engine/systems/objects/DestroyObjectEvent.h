#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		DestroyObjectEvent.h
Purpose:		Signals that an object should be safely removed from
				containers and destroyed.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-24
*************************************************************************/
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
