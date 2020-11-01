/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		GameComponent.h
Purpose:		Base class for game components
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-15
*************************************************************************/
#pragma once

#include <rapidjson/document.h>

#include <string>

namespace VannoEngine {
	class GameObject;

	class GameComponent
	{
	public:
		GameComponent(GameObject* owner) : mpOwner(owner) {}
		virtual ~GameComponent() {};
		virtual std::string GetType() = 0;

		virtual void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) = 0;

		virtual void Update(double deltaTime) {}

		GameObject* GetOwner() { return mpOwner; }
	private:
		GameObject* mpOwner;
	};

}