#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		GameObjectFactory.h
Purpose:		Handles the creation of new game objects.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-15
*************************************************************************/

#include "engine/systems/events/EventHandler.h"
#include "engine/systems/events/Event.h"

#include <rapidjson/document.h>
#include <string>
#include <unordered_map>
#include <list>

namespace VannoEngine {
	class GameObject;
	class GameComponent;
	class ComponentCreatorInterface;

	class GameObjectFactory : public EventHandler
	{
	public:
		static GameObjectFactory* GetInstance();
		~GameObjectFactory();

		void HandleEvent(std::string eventName, Event* event) override;

		void Init();

		void RegisterComponent(const std::string name, ComponentCreatorInterface* pCreator);

		GameObject* CreateObject(const std::string relativeFilePath);

		bool HasObjects() { return !mObjects.empty(); }
		/*
		std::list<GameObject*>::iterator ObjectsBegin();
		std::list<GameObject*>::iterator ObjectsEnd();
		*/
	private:
		GameObjectFactory();

		GameObject* BuildObject(const std::string relativeFilePath);
	private:
		static GameObjectFactory* mpInstance;

		std::unordered_map<std::string, ComponentCreatorInterface*> mCreators;
		std::list<GameObject*> mObjects;
	};
}