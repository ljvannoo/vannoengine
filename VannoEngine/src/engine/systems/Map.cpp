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
#include "TileMapLayer.h"
#include "Tileset.h"

#include "engine/core/components/Camera.h"

#include "engine/systems/graphics/ShaderProgram.h"

#include "engine/Log.h"

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
		// TODO delete layers
		// TODO delete tilesets
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
				mTilesets.push_back(pTileset);
			}
		}

		if (pData->HasMember("layers") && (*pData)["layers"].IsArray()) {
			const rapidjson::Value& layers = (*pData)["layers"];
			for (rapidjson::SizeType i = 0; i < layers.Size(); i++) {
				const rapidjson::Value& layerData = layers[i];

				if (layerData.HasMember("type") && layerData["type"].IsString()) {
					std::string layerType = layerData["type"].GetString();

					MapLayer* pMapLayer = NULL;
					if (layerType == IMAGE_LAYER) {
						pMapLayer = new ImageMapLayer();
					} else if (layerType == TILE_LAYER) {
						pMapLayer = new TileMapLayer();
						TileMapLayer* pTileMapLayer = static_cast<TileMapLayer*>(pMapLayer);
						pTileMapLayer->AddTilesets(&mTilesets);
					}

					if(pMapLayer) {
						pMapLayer->LoadData(&layerData);
						mLayers.push_back(pMapLayer);
					}
				}
			}
		}
	}

	void Map::Draw(Camera* pCamera) {
		for (auto it : mLayers) {
			ShaderProgram* pShaderProgram = it->GetShaderProgram();

			if(pShaderProgram) {
				pShaderProgram->Use();

				float w = pCamera->GetScreenWidth();
				float h = pCamera->GetScreenHeight();
				
				glm::mat4 projection = pCamera->GetProjectionMatrix();
				GLuint loc = pShaderProgram->GetUniformLocation("projection");
				glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

				it->Draw();

				pShaderProgram->Unuse();
			}
		}
	}
}