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

#include <glm/vec2.hpp>
#include <rapidjson/document.h>

#include <string>
#include <vector>

namespace VannoEngine {
	// Forward declarations
	class Tileset;
	class PhysicsBody;

	class TileMapLayer : public MapLayer
	{
	public: // Variables

	public: // Methods
		TileMapLayer(int tileWidth, int tileHeight);
		~TileMapLayer() override;

		std::string GetType() override { return TILE_LAYER; }

		void LoadData(const rapidjson::Value* pData) override;

		void AddTilesets(std::vector<Tileset*>* pTilesets) { mpTilesets = pTilesets; }

		void Update(double deltaTime) override;
		void Draw() override;

		void CheckCollisions(PhysicsBody* pBody) override;
	private: // Methods

	private: // Variables
		int mTileWidth;
		int mTileHeight;

		int mCols;
		
		std::vector<int> mData;
		std::vector<Tileset*>* mpTilesets;
		bool mSolid;
	};
}
