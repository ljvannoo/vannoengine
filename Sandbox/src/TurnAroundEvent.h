#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		TurnAroundEvent.h
Purpose:		Used to signal that an AI object should turn around
				win state when a player passes through it.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-29
*************************************************************************/
#include "engine/systems/events/Event.h"

#define EVT_TURN_AROUND "evt_turn_around"

namespace VannoEngine {
	class GameObject;
}
class TurnAroundEvent : public VannoEngine::Event
{
public: // Methods
	TurnAroundEvent(VannoEngine::GameObject* pObj) :
		Event(EVT_TURN_AROUND),
		mpObj{ pObj }
	{ }
	~TurnAroundEvent() {}

	VannoEngine::GameObject* GetObj() { return mpObj; }

private: // Variables
	VannoEngine::GameObject* mpObj;
};
