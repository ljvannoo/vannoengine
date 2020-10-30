#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		TileMapLayer.h
Purpose:		Describes a map layer that holds tiles
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-29
*************************************************************************/
#define TILE_LAYER "tilelayer"

#include "MapLayer.h"

#include "engine/systems/graphics/GLTexture.h"

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <rapidjson/document.h>

#include <string>
#include <vector>

namespace VannoEngine {
	// Forward declarations
	class Tileset;
	class TileMapLayer : public MapLayer
	{
	public: // Variables

	public: // Methods
		TileMapLayer();
		~TileMapLayer() override;

		std::string GetType() override { return TILE_LAYER; }

		void LoadData(const rapidjson::Value* pData) override;

		void AddTilesets(std::vector<Tileset*>* pTilesets) { mpTilesets = pTilesets; }

		void Draw() override;
	private: // Methods

	private: // Variables
		std::string mName;

		int mHeight;
		int mWidth;
		
		std::vector<int> mData;
		std::vector<Tileset*>* mpTilesets;
	};
}
