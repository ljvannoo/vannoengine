/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		PhysicsManager.cpp
Purpose:		Implementation of a physics manager
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-02
*************************************************************************/
#include "PhysicsManager.h"

#include "engine/systems/levels/LevelManager.h"
#include "engine/systems/levels/Level.h"

#include "engine/systems/physics/Collision.h"

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

	Collision PhysicsManager::CollidesWithMap(PhysicsBody* pBody) {
		LevelManager* pLevelManager = LevelManager::GetInstance();
		Level* pLevel = pLevelManager->GetCurrentLevel();
		return pLevel->Collides(pBody);
	}
}
