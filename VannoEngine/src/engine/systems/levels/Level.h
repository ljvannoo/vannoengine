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
#pragma once



#include "engine/systems/objects/GameObject.h"
#include "engine/components/Camera.h"

#include <rapidjson/document.h>

#include <string>
#include <vector>

namespace VannoEngine {
	class Map;
	class AABB;
	struct Collision;

	class Level
	{
	public: // Variables

	public: // Methods
		Level();
		~Level();

		void Init(rapidjson::Document* pLevelData);

		void UpdatePhysics(double deltaTime);
		void Update(double deltaTime);

		void Draw();

		Camera* GetCamera() { return static_cast<Camera*>(mpCamera->GetComponent(CAMERA_COMPONENT)); }

		Collision const& Collides(AABB const& aabb);
	private: // Methods

	private: // Variables
		std::string mName;

		GameObject* mpCamera;
		Map* mpMap;
	};

}