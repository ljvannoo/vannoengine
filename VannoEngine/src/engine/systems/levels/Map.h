#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Map.h
Purpose:
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-29
*************************************************************************/
#include <rapidjson/document.h>

#include <vector>

namespace VannoEngine {
	// Forward declarations
	class Tileset;
	class MapLayer;
	class PhysicsBody;
	class Collision;

	class Map 
	{
	public: // Variables

	public: // Methods
		Map();
		~Map();

		void LoadData(const rapidjson::Document* pData);

		void UpdatePhysics(double deltaTime);
		void Update(double deltaTime);
		void Draw();

		float GetWidth() { return (float)(mWidth * mTileWidth); }
		float GetHeight() { return (float)(mHeight * mTileHeight); }

		Collision Collides(PhysicsBody* pBody);
	private: // Methods

	private: // Variables
		int mHeight;		// Height of the map, in tiles
		int mWidth;			// Width of the map, in tiles
		int mTileHeight;	// How high an individual tile is, in pixels
		int mTileWidth;		// How wide an individual tile is, in pixels

		std::vector<Tileset*> mTilesets;
		std::vector<MapLayer*> mLayers;
	};
}
