/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		GameObjectFactory.cpp
Purpose:		Handles the creation of new game objects.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-15
*************************************************************************/
#include "engine/core/Log.h"

#include "engine/systems/objects/GameObjectFactory.h"
#include "engine/systems/objects/GameObject.h"

#include "engine/systems/ResourceManager.h"
#include "engine/systems/graphics/GraphicsManager.h"

#include "engine/components/GameComponent.h"
#include "engine/components/Sprite.h"
#include "engine/components/ComponentCreator.h"

#include <rapidjson/document.h>

namespace VannoEngine {
	GameObjectFactory* GameObjectFactory::mpInstance = nullptr;

	GameObjectFactory* GameObjectFactory::GetInstance() {
		if (!mpInstance) {
			mpInstance = new GameObjectFactory;
		}

		return mpInstance;
	}

	GameObjectFactory::GameObjectFactory() {

	}

	GameObjectFactory::~GameObjectFactory() {
		for (auto it : mObjects) {
			delete it;
		}
		mObjects.clear();

		for (auto it : mCreators) {
			delete it.second;
		}
		mCreators.clear();
	}

	void GameObjectFactory::Init() {
	}

	void GameObjectFactory::RegisterComponent(const std::string name, ComponentCreatorInterface* pCreator) {
		mCreators[name] = pCreator;
	}

	GameObject* GameObjectFactory::BuildObject(const std::string relativeFilePath) {
		ResourceManager* pResourceManager = ResourceManager::GetInstance();
		rapidjson::Document* pData = pResourceManager->LoadJsonData(relativeFilePath);

		GameObject* pObject = nullptr;

		if (pData->HasMember("parentObjectFile") && (*pData)["parentObjectFile"].IsString()) {
			pObject = BuildObject((*pData)["parentObjectFile"].GetString());
		}
		else {
			pObject = new GameObject();
		}

		if (pData->HasMember("name") && (*pData)["name"].IsString()) {
			pObject->SetName((*pData)["name"].GetString());
		}

		if (pData->HasMember("components") && (*pData)["components"].IsArray()) {
			const rapidjson::Value& components = (*pData)["components"];

			for (rapidjson::SizeType i = 0; i < components.Size(); i++) {
				const rapidjson::Value& componentData = components[i];
				ComponentCreatorInterface* pComponentCreator = mCreators[componentData["type"].GetString()];
				GameComponent* pComponent = pObject->GetComponent(componentData["type"].GetString());
				if (pComponent) {
					pComponent->LoadData(&componentData["data"].GetObject());
				}
				else {
					pComponent = pComponentCreator->Create(pObject, &componentData["data"].GetObject());
					pObject->AttachComponent(pComponent);
				}

			}
		}

		if (pData->HasMember("childObjectFiles") && (*pData)["childObjectFiles"].IsArray()) {
			const rapidjson::Value& childObjectFiles = (*pData)["childObjectFiles"];

			for (rapidjson::SizeType i = 0; i < childObjectFiles.Size(); i++) {
				const rapidjson::Value& childObjectFile = childObjectFiles[i];

				if (childObjectFile.IsString()) {
					GameObject* pChildObject = CreateObject(childObjectFile.GetString());
					pChildObject->SetParentObject(pObject);
					pObject->AddChildObject(pChildObject);
				}
			}
		}

		return pObject;
	}

	GameObject* GameObjectFactory::CreateObject(const std::string relativeFilePath) {
		GameObject* pObject = BuildObject(relativeFilePath);
		if (pObject) {
			mObjects.push_front(pObject);
		}

		return pObject;
	}

	std::forward_list<GameObject*>::iterator GameObjectFactory::ObjectsBegin() {
		return mObjects.begin();
	}

	std::forward_list<GameObject*>::iterator GameObjectFactory::ObjectsEnd() {
		return mObjects.end();
	}
}