#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		DetectedEnemyEvent.h
Purpose:		Used to signal that an enemy has passed within sensor range
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-29
*************************************************************************/
#include "engine/systems/events/Event.h"

#define EVT_DETECTED_ENEMY "evt_detected_enemy"

namespace VannoEngine {
	class GameObject;
}
class DetectedEnemyEvent : public VannoEngine::Event
{
public: // Methods
	DetectedEnemyEvent(VannoEngine::GameObject* mObj, VannoEngine::GameObject* pEnemy) :
		Event(EVT_DETECTED_ENEMY),
		mpObj{ mObj },
		mpEnemy{ pEnemy }
	{ }
	~DetectedEnemyEvent() {}

	VannoEngine::GameObject* GetObj() { return mpObj; }
	VannoEngine::GameObject* GetEnemy() { return mpEnemy; }

private: // Variables
	VannoEngine::GameObject* mpObj;
	VannoEngine::GameObject* mpEnemy;
};
