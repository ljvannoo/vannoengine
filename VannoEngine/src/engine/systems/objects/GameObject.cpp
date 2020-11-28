/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		GameObject.cpp
Purpose:		Main object class that provides a container for game components
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-24
*************************************************************************/
#include "engine/core/Log.h"

#include "GameObject.h"
#include "engine/components/GameComponent.h"
#include "engine/components/PhysicsBody.h"

#include "engine/systems/events/EventManager.h"
#include "engine/systems/events/Event.h"
#include "engine/systems/objects/DestroyObjectEvent.h"

#include "engine/util/uuid.h"

namespace VannoEngine {
	GameObject::GameObject(ObjectMapLayer* pMapLayer) : mpMapLayer{ pMapLayer }, mName{ "" }, mpParentObject{ nullptr }
	{
		mUuid = UUID::Generate();
	}

	GameObject::~GameObject() {
		for (auto pair : mComponents) {
			delete pair.second;
		}
		mComponents.clear();
	}

	void GameObject::UpdatePhysics(double deltaTime) {
		for (auto pair : mComponents) {
			if (pair.first == PHYSICSBODY_COMPONENT) {
				pair.second->Update(deltaTime);
			}
		}

		for (GameObject* pObject : mChildObjects) {
			pObject->UpdatePhysics(deltaTime);
		}
	}

	void GameObject::Update(double deltaTime) {
		for (auto pair : mComponents) {
			if(pair.first != PHYSICSBODY_COMPONENT) {
				pair.second->Update(deltaTime);
			}
		}

		for (GameObject* pObject : mChildObjects) {
			pObject->Update(deltaTime);
		}
	}

	void GameObject::AttachComponent(GameComponent* pComponent) {
		// Using a map to store components means that objects can only have one component of a given type.
		// So, if the component already exists, delete it
		if(pComponent) {
			if (mComponents[pComponent->GetType()]) {
				GameComponent* pComp = mComponents[pComponent->GetType()];
				mComponents.erase(pComponent->GetType());
				delete mComponents[pComponent->GetType()];
			}
			mComponents[pComponent->GetType()] = pComponent;
		}
		else {
			LOG_CORE_ERROR("Unable to attach a null component to '{0}'", mName);
		}
	}

	GameComponent* GameObject::GetComponent(std::string componentName) {
		if(HasComponent(componentName)) {
			return mComponents[componentName];
		}
		else {
			return nullptr;
		}
	}

	bool GameObject::HasComponent(std::string componentName) {
		return (mComponents.find(componentName) != mComponents.end());
	}

	void GameObject::Draw() {
		for (auto pair : mComponents) {
			pair.second->Draw();
		}
		/*if (HasComponent("sprite")) {
			GetComponent("sprite")->Draw();
		}*/
		/*
		for (GameObject* pObject : mChildObjects) {
			pObject->Draw();
		}
		*/
	}

	void GameObject::Destroy() {
		DestroyObjectEvent* pEvent = new DestroyObjectEvent(this);
		EventManager::GetInstance()->Broadcast(pEvent);
	}

	void GameObject::HandleLocalEvent(std::string eventName, Event* event) {
		for (auto pair : mComponents) {
			pair.second->HandleLocalEvent(eventName, event);
			delete event;
		}
	}
}