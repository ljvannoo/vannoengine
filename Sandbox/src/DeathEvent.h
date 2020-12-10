#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		DeathEvent.h
Purpose:		Event for signalling the death of character
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-29
*************************************************************************/

#include "engine/systems/events/Event.h"

#define EVT_DEATH "evt_death"

namespace VannoEngine {
	class GameObject;
}
class DeathEvent : public VannoEngine::Event
{
public: // Methods
	DeathEvent(VannoEngine::GameObject* pObj) :
		Event(EVT_DEATH),
		mpObj{ pObj }
	{ }
	~DeathEvent() {}

	VannoEngine::GameObject* GetObj() { return mpObj; }

private: // Variables
	VannoEngine::GameObject* mpObj;
};
