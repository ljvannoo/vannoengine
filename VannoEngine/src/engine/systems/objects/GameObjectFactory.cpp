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

#include "engine/systems/events/EventManager.h"
#include "engine/systems/objects/DestroyObjectEvent.h"

#include "engine/systems/objects/GameObjectFactory.h"
#include "engine/systems/objects/GameObject.h"

#include "engine/systems/ResourceManager.h"
#include "engine/systems/graphics/GraphicsManager.h"

#include "engine/components/GameComponent.h"
#include "engine/components/Sprite.h"
#include "engine/components/ComponentCreator.h"

#include "engine/systems/levels/ObjectMapLayer.h"

#include "engine/systems/TimeManager.h"

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
		EventManager::GetInstance()->Subscribe(EVT_DESTROY_OBJECT, this);
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

	GameObject* GameObjectFactory::BuildObject(const std::string relativeFilePath, ObjectMapLayer* pMapLayer) {
		ResourceManager* pResourceManager = ResourceManager::GetInstance();
		rapidjson::Document* pData = pResourceManager->LoadJsonData(relativeFilePath);

		GameObject* pObject = nullptr;

		if (pData->HasMember("templateObjectFile") && (*pData)["templateObjectFile"].IsString()) {
			pObject = BuildObject((*pData)["templateObjectFile"].GetString(), pMapLayer);
		}
		else {
			pObject = new GameObject(pMapLayer);
		}

		if (pData->HasMember("name") && (*pData)["name"].IsString()) {
			pObject->SetName((*pData)["name"].GetString());
		}

		if (pData->HasMember("components") && (*pData)["components"].IsArray()) {
			const rapidjson::Value& components = (*pData)["components"];

			for (rapidjson::SizeType i = 0; i < components.Size(); i++) {
				const rapidjson::Value& componentData = components[i];
				LOG_CORE_INFO("Loading component {0} for object {1}", componentData["type"].GetString(), pObject->GetName());
				ComponentCreatorInterface* pComponentCreator = mCreators[componentData["type"].GetString()];
				if (pComponentCreator) {

					GameComponent* pComponent = pObject->GetComponent(componentData["type"].GetString());
					if (pComponent) {
						if (componentData.HasMember("data")) {
							pComponent->LoadData(&componentData["data"].GetObject());
						}
					}
					else {
						if (componentData.HasMember("data")) {
							pComponent = pComponentCreator->Create(pObject, &componentData["data"].GetObject());
						}
						else {
							pComponent = pComponentCreator->Create(pObject, nullptr);
						}
						pObject->AttachComponent(pComponent);
					}
				}
				else {
					LOG_CORE_ERROR("Unable to find creator for {} component", componentData["type"].GetString());
				}
			}
		}

		if (pData->HasMember("childObjectFiles") && (*pData)["childObjectFiles"].IsArray()) {
			const rapidjson::Value& childObjectFiles = (*pData)["childObjectFiles"];

			for (rapidjson::SizeType i = 0; i < childObjectFiles.Size(); i++) {
				const rapidjson::Value& childObjectFile = childObjectFiles[i];

				if (childObjectFile.IsString()) {
					GameObject* pChildObject = CreateObject(childObjectFile.GetString(), pMapLayer);
					pChildObject->SetParentObject(pObject);
					pObject->AddChildObject(pChildObject);
				}
			}
		}
		if(pObject->GetMapLayer()) {
			pObject->GetMapLayer()->AddObject(pObject);
		}

		return pObject;
	}

	GameObject* GameObjectFactory::CreateObject(const std::string relativeFilePath, ObjectMapLayer* pMapLayer) {
		GameObject* pObject = BuildObject(relativeFilePath, pMapLayer);
		if (pObject) {
			mObjects.push_front(pObject);
		}

		return pObject;
	}
	/*
	std::list<GameObject*>::iterator GameObjectFactory::ObjectsBegin() {
		return mObjects.begin();
	}

	std::list<GameObject*>::iterator GameObjectFactory::ObjectsEnd() {
		return mObjects.end();
	}
	*/
	void GameObjectFactory::HandleEvent(std::string eventName, Event* event) {
		if (event->GetName() == EVT_DESTROY_OBJECT) {
			DestroyObjectEvent* pEvent = dynamic_cast<DestroyObjectEvent*>(event);

			GameObject* pObj = pEvent->GetObj();
			mObjects.remove(pObj);
			
			// Move destory objects to a garbage list to keep them alive for any remaining events to resolve this frame.
			// Garbage items will be collected later
			unsigned long time = TimeManager::GetInstance()->GetElapsedMillis();
			time += 500;
			mGarbage.push_back(std::make_pair(time, pObj));
		}
	}

	void GameObjectFactory::CollectGarbage() {
		unsigned long time = TimeManager::GetInstance()->GetElapsedMillis();

		for (auto it : mGarbage) {
			if (it.first > time) {
				mGarbage.remove(it);
				LOG_CORE_DEBUG("Deleting object '{}' ({})", it.second->GetName(), it.second->GetUuid());
				delete it.second;
				break; // To keep things simple, only delete one thing per frame
			}
		}
	}
}