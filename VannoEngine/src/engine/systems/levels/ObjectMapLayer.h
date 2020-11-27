#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		ObjectMapLayer.h
Purpose:		A map layer that holds objects.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-31
*************************************************************************/

#define OBJECT_LAYER "objectgroup"
#include "MapLayer.h"

#include "engine/systems/events/EventHandler.h"
#include "engine/systems/events/Event.h"

#include <string>
#include <list>

namespace VannoEngine {
	class GameObject;
	class PhysicsBody;
	class Event;

	class ObjectMapLayer : public MapLayer, public EventHandler
	{
	public: // Variables

	public: // Methods
		ObjectMapLayer(float width, float height);
		~ObjectMapLayer() override;

		std::string GetType() override { return OBJECT_LAYER; }

		void LoadData(const rapidjson::Value* pData) override;
		
		void UpdatePhysics(double deltaTime) override;
		void Update(double deltaTime) override;
		void Draw() override;
		void CheckCollisions(PhysicsBody* pBody) override;

		void HandleEvent(std::string eventName, Event* event) override;

		void AddObject(GameObject* pObject);

	private: // Methods

	private: // Variables
		std::list<GameObject*> mObjects;
	};
}
