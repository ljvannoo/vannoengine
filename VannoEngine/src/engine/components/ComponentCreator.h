#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		ComponentCreator.h
Purpose:		Template class for creating components
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-28
*************************************************************************/
#include "GameComponent.h"

namespace VannoEngine {
	class ComponentCreatorInterface {
	public:
		virtual GameComponent* Create(GameObject* owner, const rapidjson::GenericObject<true, rapidjson::Value>* pData) = 0;
	};

	template <class T>
	class ComponentCreator : public ComponentCreatorInterface {
	public:
		ComponentCreator() {}
		~ComponentCreator() {}

		GameComponent* Create(GameObject* owner, const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
			GameComponent* pComponent = static_cast<GameComponent*>(new T(owner));
			pComponent->LoadData(pData);
			return pComponent;
		}
	};
}