#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		InvulernalEvent.h
Purpose:		Used to signal the player is temporarily invulnerable
				(Used to prevent damage during attacks)
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-29
*************************************************************************/

#include "engine/systems/events/Event.h"

#define EVT_INVULNERABLE "evt_invulnerable"

namespace VannoEngine {
	class GameObject;
}
class InvulnerableEvent : public VannoEngine::Event
{
public: // Methods
	InvulnerableEvent(VannoEngine::GameObject* pObj, bool state) :
		Event(EVT_INVULNERABLE),
		mpObj{ pObj },
		mState{ state }
	{ }
	~InvulnerableEvent() {}

	VannoEngine::GameObject* GetObj() { return mpObj; }
	bool GetState() { return mState; }
private: // Variables
	VannoEngine::GameObject* mpObj;
	bool mState;
};
