/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		GameObject.h
Purpose:		Main object class that provides a container for game components
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-24
*************************************************************************/
#pragma once

#include <string>
#include <unordered_map>
#include <vector>


namespace VannoEngine {
	class GameComponent;

	class GameObject
	{
	public:
	public:
		GameObject();
		~GameObject();

		void Update(double deltaTime);

		void SetName(std::string name) { mName = name; }
		std::string GetName() { return mName; }

		std::string GetUuid() { return mUuid; }

		void AttachComponent(GameComponent* pComponent);
		GameComponent* GetComponent(std::string componentName);
		bool HasComponent(std::string componentName);

		void SetParentObject(GameObject* pParentObject) { mpParentObject = pParentObject; }
		GameObject* GetParentObject() { return mpParentObject; }

		void AddChildObject(GameObject* pChildObject) { mChildObjects.push_back(pChildObject); }

	private:
		std::string mName;
		std::unordered_map<std::string, GameComponent*> mComponents;
		GameObject* mpParentObject;
		std::vector<GameObject*> mChildObjects;

		std::string mUuid;
	};
}