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
#include "engine/Log.h"

#include "GameObject.h"
#include "engine/core/components/GameComponent.h"

#include "engine/util/uuid.h"

namespace VannoEngine {
	GameObject::GameObject() : mName(""), mpParentObject(nullptr)
	{
		mUuid = UUID::Generate();
	}

	GameObject::~GameObject() {
		for (auto pair : mComponents) {
			delete pair.second;
		}
		mComponents.clear();
	}

	void GameObject::Update(double deltaTime) {
		for (auto pair : mComponents) {
			if(pair.second) {
				pair.second->Update(deltaTime);
			}
			else {
				LOG_CORE_ERROR("Component '{0}' on object '{1}' is null!", pair.first, mName);
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
		return mComponents[componentName];
	}

	bool GameObject::HasComponent(std::string componentName) {
		return (mComponents[componentName] != nullptr);
	}
}