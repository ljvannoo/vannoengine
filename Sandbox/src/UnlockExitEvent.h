#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		UnlockExitEvent.h
Purpose:		Used to signal that the exit should be unlocked.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Dec-10
*************************************************************************/

#include "engine/systems/events/Event.h"

#define EVT_UNLOCK_EXIT "evt_unlock_exit"

class UnlockExitEvent : public VannoEngine::Event
{
public: // Methods
	UnlockExitEvent() :
		Event(EVT_UNLOCK_EXIT)
	{ }
	~UnlockExitEvent() {}
private: // Variables
};
