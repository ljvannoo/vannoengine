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
			pair.second->Update(deltaTime);
		}

		for (GameObject* pObject : mChildObjects) {
			pObject->Update(deltaTime);
		}
	}

	void GameObject::AttachComponent(GameComponent* pComponent) {
		// Using a map to store components means that objects can only have one component of a given type.
		// So, if the component already exists, delete it
		if (mComponents[pComponent->GetType()]) {
			GameComponent* pComp = mComponents[pComponent->GetType()];
			mComponents.erase(pComponent->GetType());
			delete mComponents[pComponent->GetType()];
		}
		mComponents[pComponent->GetType()] = pComponent;

	}

	GameComponent* GameObject::GetComponent(std::string componentName) {
		return mComponents[componentName];
	}

	bool GameObject::HasComponent(std::string componentName) {
		return (mComponents[componentName] != nullptr);
	}
}