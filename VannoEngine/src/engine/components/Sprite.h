/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Sprite.h
Purpose:		Provides functionality for managing a very basic sprite
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-08
*************************************************************************/
#pragma once

#define SPRITE_COMPONENT "sprite"

#include <glm/vec2.hpp>

#include <rapidjson/document.h>

#include "engine/components/GameComponent.h"

namespace VannoEngine {
	class Surface;

	class Sprite : public GameComponent
	{
	public:
		Sprite(GameObject* owner);
		~Sprite() override;

		std::string GetType() override {
			return SPRITE_COMPONENT;
		}

		void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

		void Update(double deltaTime) override;
		void Draw() override;

		bool IsHorizontalFlipped() { return mFlipHorizontal; }
		void SetFlipHorizontal(bool flip) { mFlipHorizontal = flip; }
		void FlipHorizontal() {
			mFlipHorizontal = !mFlipHorizontal;
		}

		float GetZOrder() const { return mZOrder; }

		float GetHeight();
		float GetWidth();
	private:
		Surface* mpSurface;

		int mSheetRows;
		int mSheetCols;

		bool mFlipHorizontal;

		float mZOrder;

		bool mUiMode;
	};
}