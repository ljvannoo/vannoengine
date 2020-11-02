/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		PhysicsManager.h
Purpose:		Manages physics for the game
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-01
*************************************************************************/
#include "PhysicsManager.h"

namespace VannoEngine {
	PhysicsManager* PhysicsManager::mpInstance = nullptr;

	PhysicsManager* PhysicsManager::GetInstance() {
		if (!mpInstance) {
			mpInstance = new PhysicsManager;
		}

		return mpInstance;
	}

	PhysicsManager::PhysicsManager() {

	}

	PhysicsManager::~PhysicsManager() {

	}
}
