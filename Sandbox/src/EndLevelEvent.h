#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		EndLevelEvent.h
Purpose:		Used to signal the main game that the level has ended and
				should be unloaded.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-30
*************************************************************************/
#include "engine/systems/events/Event.h"

#define EVT_END_LEVEL "evt_end_level"

namespace VannoEngine {
	class GameObject;
}
class EndLevelEvent : public VannoEngine::Event
{
public: // Methods
	EndLevelEvent() :
		Event(EVT_END_LEVEL)
	{ }
	~EndLevelEvent() {}
private: // Variables
};
