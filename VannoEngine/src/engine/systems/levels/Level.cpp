/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Level.h
Purpose:		Contains the objects and camera for a specific level
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-21
*************************************************************************/
#include "Level.h"

#include "engine/core/Log.h"

#include "engine/systems/physics/Aabb.h"

#include "engine/systems/objects/GameObjectFactory.h"
#include "engine/systems/ResourceManager.h"

#include "Map.h"

#include "engine/components/GameComponent.h"

namespace VannoEngine {
	Level::Level() :
		mpCamera(nullptr),
		mpMap(nullptr)
	{ }

	Level::~Level() {
		delete mpMap;
	}


	void Level::Init(rapidjson::Document* pLevelData) {
		GameObjectFactory* pObjectFactory = GameObjectFactory::GetInstance();

		if (pLevelData->HasMember("name") && (*pLevelData)["name"].IsString()) {
			mName = (*pLevelData)["name"].GetString();
		}

		if (pLevelData->HasMember("camera") && (*pLevelData)["camera"].IsString()) {
			mpCamera = pObjectFactory->CreateObject((*pLevelData)["camera"].GetString(), nullptr);
		}

		if (pLevelData->HasMember("map") && (*pLevelData)["map"].IsString()) {
			ResourceManager* pResourceManager = ResourceManager::GetInstance();
			LOG_CORE_INFO("Loading map from '{0}'", (*pLevelData)["map"].GetString());
			rapidjson::Document* mapData = pResourceManager->LoadJsonData((*pLevelData)["map"].GetString());

			// Create the map
			mpMap = new Map();
			mpMap->LoadData(mapData);
		}
	}

	void Level::UpdatePhysics(double deltaTime) {
		mpMap->UpdatePhysics(deltaTime);
	}

	void Level::Update(double deltaTime) {
		mpCamera->Update(deltaTime);
		mpMap->Update(deltaTime);
	}

	void Level::Draw() {
		mpMap->Draw();

		for (auto it : mUiObjects) {
			it->Draw();
		}
	}

	void Level::CheckCollisions(PhysicsBody* pBody) {
		return mpMap->CheckCollisions(pBody);
	}

	float Level::GetWidth() {
		return mpMap->GetWidth();
	}

	float Level::GetHeight() {
		return mpMap->GetHeight();
	}

	void Level::AddUiObject(GameObject* pObj) {
		mUiObjects.push_back(pObj);
	}

	void Level::RemoveUiObject(GameObject* pObj) {
		mUiObjects.remove(pObj);
	}
}
