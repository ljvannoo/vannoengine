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
		if (mpMap) {
			delete mpMap;
			for (auto it : mUiObjects) {
				it->Destroy();
			}
		}
	}


	void Level::LoadData(rapidjson::Document* pLevelData) {
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

		if (pLevelData->HasMember("uiObjects") && (*pLevelData)["uiObjects"].IsArray()) {
			const rapidjson::Value& uiObjects = (*pLevelData)["uiObjects"];
			for (rapidjson::SizeType i = 0; i < uiObjects.Size(); i++) {
				const rapidjson::Value& uiObject = uiObjects[i];

				if (uiObject.IsString()) {
					mUiObjects.push_back(pObjectFactory->CreateObject(uiObject.GetString(), nullptr));
				}
			}
		}
	}

	void Level::UpdatePhysics(double deltaTime) {
		if(mpMap) {
			mpMap->UpdatePhysics(deltaTime);
		}
	}

	void Level::Update(double deltaTime) {
		mpCamera->Update(deltaTime);
		if (mpMap) {
			mpMap->Update(deltaTime);
		}

		for (auto it : mUiObjects) {
			it->Update(deltaTime);
		}
	}

	void Level::Draw() {
		if (mpMap) {
			mpMap->Draw();
		}

		for (auto it : mUiObjects) {
			it->Draw();
		}
	}

	void Level::CheckCollisions(PhysicsBody* pBody) {
		return mpMap->CheckCollisions(pBody);
	}

	float Level::GetWidth() {
		if (mpMap) {
			return mpMap->GetWidth();
		}
		return 0.0f;
	}

	float Level::GetHeight() {
		if (mpMap) {
			return mpMap->GetHeight();
		}
		return 0.0f;
	}

	void Level::AddUiObject(GameObject* pObj) {
		mUiObjects.push_back(pObj);
	}

	void Level::RemoveUiObject(GameObject* pObj) {
		mUiObjects.remove(pObj);
	}
}
