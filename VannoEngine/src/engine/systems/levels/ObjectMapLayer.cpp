/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		ObjectMapLayer.cpp
Purpose:		Implementation of an ObjectMapLayer
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-31
*************************************************************************/

#include "ObjectMapLayer.h"

#include "engine/core/Log.h"

#include "engine/systems/objects/GameObjectFactory.h"

#include "engine/systems/objects/GameObject.h"
#include "engine/components/Transform.h"
#include "engine/components/PhysicsBody.h"
#include "engine/components/Sprite.h"

#include "engine/systems/events/EventManager.h"
#include "engine/systems/objects/DestroyObjectEvent.h"

namespace VannoEngine {
	ObjectMapLayer::ObjectMapLayer(float width, float height) : 
		MapLayer()
	{
		SetDimensions(width, height);
		EventManager::GetInstance()->Subscribe(EVT_DESTROY_OBJECT, this);
	}

	ObjectMapLayer::~ObjectMapLayer() {

	}

	void ObjectMapLayer::LoadData(const rapidjson::Value* pData)
	{
		if (pData->HasMember("name") && (*pData)["name"].IsString()) {
			mName = (*pData)["name"].GetString();
			LOG_CORE_INFO("Loading object map layer '{0}'", mName);
		}

		float x = 0.0f;
		if (pData->HasMember("x") && (*pData)["x"].IsNumber()) {
			x = (*pData)["x"].GetFloat();
		}

		float y = 0.0f;
		if (pData->HasMember("y") && (*pData)["y"].IsNumber()) {
			y = (*pData)["y"].GetFloat();
		}
		SetPosition(x, y);

		if (pData->HasMember("objects") && (*pData)["objects"].IsArray()) {
			GameObjectFactory* pObjectFactory = GameObjectFactory::GetInstance();

			// Load objects
			const rapidjson::Value& objects = (*pData)["objects"];
			for (rapidjson::SizeType i = 0; i < objects.Size(); i++) {
				const rapidjson::Value& object = objects[i];
				GameObject* pGameObject = nullptr;

				// Load properties for this object
				if (object.HasMember("properties") && object["properties"].IsArray()) {
					const rapidjson::Value& properties = object["properties"];
					for (rapidjson::SizeType i = 0; i < properties.Size(); i++) {
						const rapidjson::Value& property = properties[i];

						if (property.HasMember("name") && property["name"].IsString() && std::string(property["name"].GetString()) == "objectFile" &&
							property.HasMember("value") && property["value"].IsString()) {
							pGameObject = pObjectFactory->CreateObject(property["value"].GetString(), this);
						}
					}
				}
				if (pGameObject) {
					if (object.HasMember("name") && object["name"].IsString()) {
						pGameObject->SetName(object["name"].GetString());
					}
					LOG_CORE_DEBUG("Loading object {} on layer {}", pGameObject->GetName(), mName);
					float width = 0.0f;
					if (object.HasMember("width") && object["width"].IsNumber()) {
						width = object["width"].GetFloat();
					}

					float height = 0.0f;
					if (object.HasMember("height") && object["height"].IsNumber()) {
						height = object["height"].GetFloat();
					}

					Transform* pTransform = static_cast<Transform*>(pGameObject->GetComponent(TRANSFORM_COMPONENT));
					if (pTransform) {
						float x = 0.0f;
						if (object.HasMember("x") && object["x"].IsNumber()) {
							x = object["x"].GetFloat();
						}

						float y = 0.0f;
						if (object.HasMember("y") && object["y"].IsNumber()) {
							y = object["y"].GetFloat();
						}
						pTransform->SetPositionY(GetUpperLeft().y - (y + height / 2.0f));
						pTransform->SetPositionX(GetUpperLeft().x + (x + width / 2.0f));
						//LOG_CORE_DEBUG("Upper left: ({0}, {1})", GetUpperLeft().x, GetUpperLeft().y);
						LOG_CORE_DEBUG("{0} is at ({1}, {2})", pGameObject->GetName(), pTransform->GetPosition().x, pTransform->GetPosition().y);
					}
					else {
						LOG_CORE_ERROR("Game object has no transform!");
					}

					if (pGameObject->HasComponent(PHYSICSBODY_COMPONENT) && object.HasMember("type") && object["type"].IsString()) {
						std::string type = object["type"].GetString();
						if(type.length() > 0) {
							PhysicsBody* pBody = dynamic_cast<PhysicsBody*>(pGameObject->GetComponent(PHYSICSBODY_COMPONENT));
							pBody->SetPhysicsLayer(type);
						}
					}
					/*
					PhysicsBody* pBody = static_cast<PhysicsBody*>(pGameObject->GetComponent(PHYSICSBODY_COMPONENT));
					if (pBody) {
						pBody->SetWidth(width);
						pBody->SetHeight(height);
					}
					*/
					//mObjects.push_back(pGameObject);
				}
				else {
					LOG_CORE_ERROR("Failed to load game object");
				}
			}
		}
	}

	void ObjectMapLayer::UpdatePhysics(double deltaTime) {
		for (GameObject* pObject : mObjects) {
			pObject->UpdatePhysics(deltaTime);
		}
	}

	void ObjectMapLayer::Update(double deltaTime) {
		for (GameObject* pObject : mObjects) {
			pObject->Update(deltaTime);
		}
	}

	void ObjectMapLayer::Draw() {
		for (GameObject* pObject : mObjects) {
			pObject->Draw();
		}
	}

	void ObjectMapLayer::CheckCollisions(PhysicsBody* pBody) {
		for (GameObject* pObject : mObjects) {
			if (pObject->HasComponent(PHYSICSBODY_COMPONENT)) {
				PhysicsBody* pOtherBody = dynamic_cast<PhysicsBody*>(pObject->GetComponent(PHYSICSBODY_COMPONENT));
				pBody->CheckCollision(pOtherBody);
			}
		}
	}

	void ObjectMapLayer::HandleEvent(std::string eventName, Event* event) {
		if (event->GetName() == EVT_DESTROY_OBJECT) {
			DestroyObjectEvent* pEvent = dynamic_cast<DestroyObjectEvent*>(event);
			GameObject* pObj = pEvent->GetObj();
			mObjects.remove(pObj);
		}
	}
	void ObjectMapLayer::AddObject(GameObject* pObject) {
		mObjects.push_back(pObject);
	}
}
