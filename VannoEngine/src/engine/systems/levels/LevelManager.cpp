/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		LevelManager.cpp
Purpose:		Manages loading and unloading levels
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-21
*************************************************************************/
#include "LevelManager.h"

#include "engine/systems/ResourceManager.h"
#include "engine/systems/graphics/GraphicsManager.h"

#include "engine/systems/levels/Level.h"
#include "engine/components/Camera.h"
#include "engine/Game.h"

#include <rapidjson/document.h>

namespace VannoEngine {
	LevelManager* LevelManager::mpInstance = nullptr;

	LevelManager* LevelManager::GetInstance() {
		if (!mpInstance) {
			mpInstance = new LevelManager;
		}

		return mpInstance;
	}

	LevelManager::LevelManager() {

	}

	LevelManager::~LevelManager() {
		while (!mLevels.empty()) {
			UnloadLevel();
		}
		if (mpInstance) {
			delete mpInstance;
		}
	}

	void LevelManager::UpdatePhysics(double deltaTime) {
		Level* pLevel = GetCurrentLevel();
		if(pLevel) {
			pLevel->UpdatePhysics(deltaTime);
		}
	}

	void LevelManager::Update(double deltaTime) {
		Level* pLevel = GetCurrentLevel();
		if (pLevel) {
			pLevel->Update(deltaTime);
		}
	}

	void LevelManager::LoadLevel(const std::string& relativePath) {
		ResourceManager* pResourceManager = ResourceManager::GetInstance();
		rapidjson::Document* pLevelData = pResourceManager->LoadJsonData(relativePath);

		Level* pLevel = new Level();
		pLevel->LoadData(pLevelData);
		mLevels.push(pLevel);
	}

	void LevelManager::UnloadLevel() {
		if (!mLevels.empty()) {
			Level* pLevel = mLevels.top();
			delete pLevel;
			mLevels.pop();
		}
	}

	Level* LevelManager::GetCurrentLevel() {
		if(mLevels.empty()) {
			return nullptr;
		}
		return mLevels.top();
	}

	Camera* LevelManager::GetCamera() {
		if (mLevels.empty()) {
			return nullptr;
		}
		return GetCurrentLevel()->GetCamera();
	}

	void LevelManager::Draw() {
		if(!mLevels.empty()) {
			GetCurrentLevel()->Draw();
		}
	}
}