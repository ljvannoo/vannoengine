#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		ImageMapLayer.h
Purpose:		Describes a map layer that holds images
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-29
*************************************************************************/
#define IMAGE_LAYER "imagelayer"

#include "MapLayer.h"

#include "engine/systems/graphics/Surface.h"

#include <glm/vec2.hpp>
#include <rapidjson/document.h>

#include <string>

namespace VannoEngine {
	// Forward declarations
	class ImageMapLayer : public MapLayer
	{
	public: // Methods
		ImageMapLayer();
		~ImageMapLayer() override;

		std::string GetType() override { return IMAGE_LAYER;  }

		void LoadData(const rapidjson::Value* pData) override;

		void Update(double deltaTime) override;
		void Draw() override;

	private: // Variables
		Surface* mpSurface;
	};
}
