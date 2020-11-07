/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Map.cpp
Purpose:		Implementation of a map
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-29
*************************************************************************/
#include "Map.h"

#include "MapLayer.h"
#include "ImageMapLayer.h"
#include "ObjectMapLayer.h"
#include "TileMapLayer.h"
#include "Tileset.h"

#include "engine/components/Camera.h"

#include "engine/systems/graphics/ShaderProgram.h"
#include "engine/systems/physics/Aabb.h"

#include "engine/core/Log.h"

#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

namespace VannoEngine {
	Map::Map() :
		mHeight(0),
		mWidth(0),
		mTileHeight(0),
		mTileWidth(0)
	{ }

	Map::~Map() {
		for (auto it : mTilesets) {
			delete it;
		}
		for (auto it : mLayers) {
			delete it;
		}
	}

	void Map::LoadData(const rapidjson::Document* pData)
	{
		if (pData->HasMember("height") && (*pData)["height"].IsInt()) {
			mHeight = (*pData)["height"].GetInt();
		}

		if (pData->HasMember("width") && (*pData)["width"].IsInt()) {
			mWidth = (*pData)["width"].GetInt();
		}

		if (pData->HasMember("tileheight") && (*pData)["tileheight"].IsInt()) {
			mTileHeight = (*pData)["tileheight"].GetInt();
		}

		if (pData->HasMember("tilewidth") && (*pData)["tilewidth"].IsInt()) {
			mTileWidth = (*pData)["tilewidth"].GetInt();
		}

		if (pData->HasMember("tilesets") && (*pData)["tilesets"].IsArray()) {
			const rapidjson::Value& tilesets = (*pData)["tilesets"];
			for (rapidjson::SizeType i = 0; i < tilesets.Size(); i++) {
				const rapidjson::Value& tilesetData = tilesets[i];

				Tileset* pTileset = new Tileset(mTileWidth, mTileHeight);
				pTileset->LoadData(&tilesetData);
				if(mTilesets.size() > 0) {
					mTilesets.back()->SetEndIndex(pTileset->GetStartIndex() - 1);
				}
				mTilesets.push_back(pTileset);
			}
		}

		if (pData->HasMember("layers") && (*pData)["layers"].IsArray()) {
			const rapidjson::Value& layers = (*pData)["layers"];
			for (rapidjson::SizeType i = 0; i < layers.Size(); i++) {
				const rapidjson::Value& layerData = layers[i];

				if (layerData.HasMember("type") && layerData["type"].IsString()) {
					std::string layerType = layerData["type"].GetString();

					// Determine which type of layer we've found and load that
					MapLayer* pMapLayer = NULL;
					if (layerType == IMAGE_LAYER) {
						pMapLayer = new ImageMapLayer();
					}
					else if (layerType == TILE_LAYER) {
						pMapLayer = new TileMapLayer(mTileWidth, mTileHeight);

						// TODO Move tilesets to ResourceManager?
						TileMapLayer* pTileMapLayer = static_cast<TileMapLayer*>(pMapLayer);
						pTileMapLayer->AddTilesets(&mTilesets);
					}
					else if (layerType == OBJECT_LAYER) {
						pMapLayer = new ObjectMapLayer(static_cast<float>(mWidth * mTileWidth), static_cast<float>(mHeight * mTileHeight));
					}

					// If the layer is of a known type, load it
					if(pMapLayer) {
						pMapLayer->LoadData(&layerData);
						mLayers.push_back(pMapLayer);
					}
				}
			}
		}
	}

	void Map::UpdatePhysics(double deltaTime) {
		for (auto it : mLayers) {
			it->UpdatePhysics(deltaTime);
		}
	}

	void Map::Update(double deltaTime) {
		for (auto it : mLayers) {
			it->Update(deltaTime);
		}
	}

	void Map::Draw() {
		for (auto it : mLayers) {
			it->Draw();
		}
	}

	Collision const& Map::Collides(AABB const& aabb) {
		Collision result;
		for (MapLayer* pLayer : mLayers) {
			result = pLayer->Collides(aabb);
			if (result.bits != 0) {
				break;
			}
		}
		return result;
	}
}