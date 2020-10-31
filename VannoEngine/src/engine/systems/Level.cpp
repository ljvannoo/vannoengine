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
#include "engine/Log.h"
#include "Level.h"

#include "GameObjectFactory.h"
#include "ResourceManager.h"

#include "Map.h"

#include "engine/core/components/GameComponent.h"

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

		if (pLevelData->HasMember("camera")) {
			const rapidjson::Value& cameraData = (*pLevelData)["camera"];

			if (cameraData.HasMember("objectFile") && cameraData["objectFile"].IsString()) {
				// Create the object with default values
				mpCamera = pObjectFactory->CreateObject(cameraData["objectFile"].GetString());

				// Load overrides
				if (cameraData.HasMember("overrides") && cameraData["overrides"].IsArray()) {
					LoadOverrides(mpCamera, cameraData["overrides"]);
				}
			}
		}

		if (pLevelData->HasMember("map") && (*pLevelData)["map"].IsString()) {
			ResourceManager* pResourceManager = ResourceManager::GetInstance();
			LOG_CORE_INFO("Loading map from '{0}'", (*pLevelData)["map"].GetString());
			rapidjson::Document* mapData = pResourceManager->LoadJsonData((*pLevelData)["map"].GetString());

			// Create the map
			mpMap = new Map();
			mpMap->LoadData(mapData);
		}

		if (pLevelData->HasMember("objects") && (*pLevelData)["objects"].IsArray()) {
			const rapidjson::Value& objects = (*pLevelData)["objects"];
			for (rapidjson::SizeType i = 0; i < objects.Size(); i++) {
				const rapidjson::Value& objectData = objects[i];

				if (objectData.HasMember("objectFile") && objectData["objectFile"].IsString()) {
					GameObject* pObject = pObjectFactory->CreateObject(objectData["objectFile"].GetString());
					mObjects.push_back(pObject);

					if (objectData.HasMember("overrides") && objectData["overrides"].IsArray()) {
						LoadOverrides(pObject, objectData["overrides"]);
					}
				}
			}
		}
	}

	void Level::LoadOverrides(GameObject* pObject, const rapidjson::Value& overrides) {
		for (rapidjson::SizeType i = 0; i < overrides.Size(); i++) {
			const rapidjson::Value& overrideData = overrides[i];
			if (overrideData.HasMember("type") && overrideData["type"].IsString()) {
				GameComponent* pComponent = pObject->GetComponent(overrideData["type"].GetString());
				if (overrideData.HasMember("data")) {
					pComponent->LoadData(&overrideData["data"].GetObject());
				}
			}
		}
	}


	void Level::Update(double deltaTime) {
		for (GameObject* pObject : mObjects) {
			pObject->Update(deltaTime);
		}
	}

	void Level::Draw() {
		mpMap->Draw(static_cast<Camera*>(mpCamera->GetComponent(CAMERA_COMPONENT)));
	}
}