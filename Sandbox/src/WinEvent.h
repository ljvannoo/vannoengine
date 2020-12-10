#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		WinEvent.h
Purpose:		Used to signal that the player has won the game.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Dec-09
*************************************************************************/

#include "engine/systems/events/Event.h"

#define EVT_WIN "evt_win"

class WinEvent : public VannoEngine::Event
{
public: // Methods
	WinEvent() :
		Event(EVT_WIN)
	{ }
	~WinEvent() {}
private: // Variables
};
