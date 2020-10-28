/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		UpDown.h
Purpose:		Simple component for moving an object up and down indefinitely
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-24
*************************************************************************/
#pragma once
#define UPDOWN_COMPONENT "updown"

#include <rapidjson/document.h>

#include "engine/core/components/GameComponent.h"

namespace VannoEngine {
	class UpDown : public GameComponent
	{
	public:
		UpDown(GameObject* owner);
		~UpDown() override;

		void Update(double deltaTime);

		void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

		std::string GetType() override {
			return UPDOWN_COMPONENT;
		}
	private:
		bool mMovingUp;
		float mMaxDistance;

		float mDistance;
	};
}