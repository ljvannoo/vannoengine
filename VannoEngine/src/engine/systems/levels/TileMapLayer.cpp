/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		TileMapLayer.cpp
Purpose:		Implementation of a TileMapLayer
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-29
*************************************************************************/
#include "TileMapLayer.h"

#include "Tileset.h"

#include "engine/systems/ResourceManager.h"
#include "engine/systems/graphics/GraphicsManager.h"
#include "engine/systems/graphics/ShaderProgram.h"
#include "engine/systems/graphics/Vertex.h"

#include "engine/systems/physics/Aabb.h"
#include "engine/systems/physics/Collision.h"

#include "engine/core/Log.h"

#include "engine/util/bitmask.h"

#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <math.h>

namespace VannoEngine {
	TileMapLayer::TileMapLayer(int tileWidth, int tileHeight) :
		MapLayer(),
		mTileWidth(tileWidth),
		mTileHeight(tileHeight),
		mpTilesets(nullptr),
		mSolid(false),
		mCols(0)
	{ }

	TileMapLayer::~TileMapLayer() {

	}

	void TileMapLayer::LoadData(const rapidjson::Value* pData) {
		if (pData->HasMember("name") && (*pData)["name"].IsString()) {
			mName = (*pData)["name"].GetString();
			LOG_CORE_INFO("Loading tile map layer '{0}'", mName);
		}

		float x = 0.0f;
		if (pData->HasMember("x") && (*pData)["x"].IsNumber()) {
			x = (*pData)["x"].GetFloat();
		}

		float y = 0.0f;
		if (pData->HasMember("y") && (*pData)["y"].IsNumber()) {
			y = (*pData)["y"].GetFloat();
		}
		SetPosition(x, y);

		float height = 1.0f;
		if (pData->HasMember("height") && (*pData)["height"].IsNumber()) {
			height = (*pData)["height"].GetFloat() * static_cast<float>(mTileHeight);
		}

		float width = 0.0f;
		if (pData->HasMember("width") && (*pData)["width"].IsNumber()) {
			width = (*pData)["width"].GetFloat() * static_cast<float>(mTileWidth);
		}
		SetDimensions(width, height);

		if (pData->HasMember("data") && (*pData)["data"].IsArray()) {
			const rapidjson::Value& cells = (*pData)["data"];
			mData.reserve((size_t)GetWidth() * (size_t)GetHeight());
			for (rapidjson::SizeType i = 0; i < cells.Size(); i++) {
				const rapidjson::Value& cell = cells[i];

				mData.push_back(cell.GetInt());
			}
		}

		if (pData->HasMember("properties") && (*pData)["properties"].IsArray()) {
			const rapidjson::Value& properties = (*pData)["properties"];

			for (rapidjson::SizeType i = 0; i < properties.Size(); i++) {
				const rapidjson::Value& property = properties[i];

				if (property.HasMember("name") && property["name"].IsString() && std::string(property["name"].GetString()) == "solid" &&
					property.HasMember("value") && property["value"].IsBool()) {
					mSolid = property["value"].GetBool();
				}
			}
		}
		mCols = mDimensions.x / mTileWidth;
	}

	void TileMapLayer::Update(double deltaTime) {

	}

	void TileMapLayer::Draw() {
		if (!mpTilesets) {
			LOG_CORE_ERROR("No tilesets available");
			return;
		}
		for (int i = 0; i < mData.size(); ++i) {
			int tileId = mData[i]-1;
			if(tileId >= 0) {
				Tileset* pTileset = nullptr;
				for (Tileset* it : *mpTilesets) {
					if (!pTileset) {
						pTileset = it;
					}
					else if (tileId > it->GetStartIndex()) {
						pTileset = it;
					}
					else {
						break;
					}
				}

				if(pTileset) {
					GraphicsManager* pGraphicsManager = GraphicsManager::GetInstance();

					glm::vec2 topLeft(0.0f, GetHeight());
					int col = i % ((int)GetWidth() / mTileWidth);
					int row = i / ((int)GetWidth() / mTileWidth);
					float x = topLeft.x + static_cast<float>(col * pTileset->GetTileWidth());
					float y = topLeft.y - static_cast<float>((row+1) * pTileset->GetTileHeight());
					glm::mat4 t(1.0f);
					t = glm::translate(t, glm::vec3(0.0f, static_cast<float>(pTileset->GetTileWidth()), 0.0f));
					t = glm::translate(t, glm::vec3(x, y, 0.0f));

					pGraphicsManager->Render(
						pTileset->GetSurface(), 
						&t, 
						static_cast<float>(pTileset->GetWidth()),
						static_cast<float>(pTileset->GetHeight()),
						static_cast<float>(pTileset->GetTileWidth()), 
						static_cast<float>(pTileset->GetTileHeight()),
						tileId,
						false);
				}
			}
		}
	}
	Collision const&  TileMapLayer::Collides(AABB const& aabb) {
		Collision result;
		if (mSolid) {
			float tH = (float)mTileHeight;
			float tW = (float)mTileWidth;
			glm::vec2 min(aabb.center.x - aabb.halfWidth, aabb.center.y - aabb.halfHeight); // Bottom left
			glm::vec2 max(aabb.center.x + aabb.halfWidth, aabb.center.y + aabb.halfHeight); // Top right

			int topRow = (int)round((GetUpperLeft().y - max.y) / tH);
			int bottomRow = (int)round((GetUpperLeft().y - min.y) / tH);
			int leftCol = (int)floor((GetUpperLeft().x + min.x) / tW);
			int rightCol = (int)floor((GetUpperLeft().x + max.x) / tW);

			int testCol, testRow, index;
			float test;
			
			// Test top and bottom
			test = min.x + tW / 2.0f;
			while (test <= max.x - tW / 2.0f) {
				testCol = (int)round((GetUpperLeft().x + test) / tW);
				
				index = topRow * mCols + testCol;
				if (index < mData.size() && mData[index] != 0) {
					SetBit(result.bits, BIT_COLLISION_TOP);
					result.top = GetUpperLeft().y - ((index / mCols) * tH) - tH;
				}
				
				index = bottomRow * mCols + testCol;
				if (index < mData.size() && mData[index] != 0) {
					SetBit(result.bits, BIT_COLLISION_BOTTOM);
					result.bottom = GetUpperLeft().y - ((index / mCols) * tH);
				}

				test += tW;
			}

			// Test left and right
			test = min.y + tH / 2.0f;
			while (test <= max.y - tH / 2.0f) {
				testRow = (int)floor((GetUpperLeft().y - test) / tH);

				index = testRow * mCols + leftCol;
				if (index < mData.size() && mData[index] != 0) {
					SetBit(result.bits, BIT_COLLISION_LEFT);
					result.left = mPosition.x + ((index % mCols) * tW) + tW;
				}

				index = testRow * mCols + rightCol;
				if (index < mData.size() && mData[index] != 0) {
					SetBit(result.bits, BIT_COLLISION_RIGHT);
					result.right = mPosition.x + ((index % mCols) * tW);
				}
				test += tH;
			}
		}

		return result;
	}
}
