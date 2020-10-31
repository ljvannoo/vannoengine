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

#include <string>
namespace VannoEngine {
	class ObjectMapLayer : public MapLayer
	{
	public: // Variables

	public: // Methods
		ObjectMapLayer(float pixelHeight);
		~ObjectMapLayer() override;

		std::string GetType() override { return OBJECT_LAYER; }

		void LoadData(const rapidjson::Value* pData) override;

		void Draw() override;

	private: // Methods

	private: // Variables
		std::string mName;

		float mHeight; // In pixels
	};
}
