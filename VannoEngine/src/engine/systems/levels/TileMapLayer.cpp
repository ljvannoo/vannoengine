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
#include "engine/systems/physics/MapCollisionEvent.h"
#include "engine/systems/events/DamageEvent.h"

#include "engine/components/PhysicsBody.h"

#include "engine/systems/levels/LevelManager.h"
#include "engine/components/Camera.h"

#include "engine/systems/events/EventManager.h"

#include "engine/systems/objects/GameObject.h"

#include "engine/core/Log.h"

#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <math.h>

namespace VannoEngine {
	TileMapLayer::TileMapLayer(int tileWidth, int tileHeight) :
		MapLayer(),
		mTileWidth{ tileWidth },
		mTileHeight{tileHeight},
		mpTilesets{nullptr},
		mSolid{false},
		mCols{0},
		mPlatform{false},
		mDamage{ 0.0f }
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
				if (property.HasMember("name") && property["name"].IsString() && std::string(property["name"].GetString()) == "platform" &&
					property.HasMember("value") && property["value"].IsBool()) {
					mPlatform = property["value"].GetBool();
				}
				if (property.HasMember("name") && property["name"].IsString() && std::string(property["name"].GetString()) == "damage" &&
					property.HasMember("value") && property["value"].IsNumber()) {
					mDamage = property["value"].GetFloat();
				}
			}
		}
		mCols = (int)(mDimensions.x / mTileWidth);
	}

	void TileMapLayer::Update(double deltaTime) {

	}

	void TileMapLayer::Draw() {
		Camera* pCamera = LevelManager::GetInstance()->GetCamera();
		GraphicsManager* pGraphicsManager = GraphicsManager::GetInstance();
		glm::vec2 topLeft = GetUpperLeft();
		glm::vec3 position(0.0f);

		if (!mpTilesets) {
			LOG_CORE_ERROR("No tilesets available");
			return;
		}

		Tileset* pTileset = nullptr;
		for (int i = 0; i < mData.size(); ++i) {
			int tileId = mData[i];
			if(tileId > 0) {
				if(!pTileset || tileId > pTileset->GetEndIndex() || tileId < pTileset->GetStartIndex()) {
					Tileset* pNewTileset = nullptr;
					for (Tileset* it : *mpTilesets) {
						if (tileId >= it->GetStartIndex() && tileId <= it->GetEndIndex()) {
							pNewTileset = it;
							break;
						}
					}

					if (pNewTileset && (!pTileset || pNewTileset->GetName() != pTileset->GetName())) {
						pTileset = pNewTileset;

						// When choosing a new tileset, start a new batch
						pGraphicsManager->StartSpriteBatch(
							pTileset->GetSurface(),
							pTileset->GetWidth(),
							pTileset->GetHeight(),
							(float)pTileset->GetTileWidth(),
							(float)pTileset->GetTileHeight());
					}
				}
				if(pTileset) {
					int col = i % mCols;
					int row = i / mCols;
					float x = topLeft.x + (float)(col * pTileset->GetTileWidth());
					float y = topLeft.y - (float)((row+1) * pTileset->GetTileHeight());
					glm::mat4 t(1.0f);
					position.x = x;
					position.y = y + (float)pTileset->GetTileHeight();


					if (pCamera->InView(glm::vec2(position.x + (float)mTileWidth, position.y + (float)mTileHeight), glm::vec2(position.x - (float)mTileWidth, position.y - (float)mTileHeight))) {
						t = glm::translate(t, position);
						int spriteIndex = tileId - pTileset->GetStartIndex();
						int cols = (int)(pTileset->GetWidth() / (float)pTileset->GetTileWidth());
						int col = spriteIndex % cols;
						int row = spriteIndex / cols;
						pGraphicsManager->BatchRender(t, col, row);
					}
				}
			}
		}

		pGraphicsManager->EndSpriteBatch();
	}

	void TileMapLayer::CheckCollisions(PhysicsBody* pBody) {
		if (mSolid || mDamage > 0.0f) {
			EventManager* pEventManager = EventManager::GetInstance();
			float tH = (float)mTileHeight;
			float tW = (float)mTileWidth;
			glm::vec2 center = pBody->GetAabbCenter();
			glm::vec2 halfDim = pBody->GetAabbHalfDimensions();
			glm::vec2 min(center.x - halfDim.x, center.y - halfDim.y); // Bottom left
			glm::vec2 max(center.x + halfDim.x, center.y + halfDim.y); // Top right

			int topRow = (int)round((GetUpperLeft().y - max.y) / tH);
			int bottomRow = (int)round((GetUpperLeft().y - min.y) / tH);
			int leftCol = (int)floor((GetUpperLeft().x + min.x) / tW);
			int rightCol = (int)floor((GetUpperLeft().x + max.x) / tW);

			int testCol, testRow, index;
			float test;

			bool collision = false;
			
			CollisionType type = CollisionType::HARD;
			if (mPlatform) {
				type = CollisionType::SOFT;
			}
			// Test top and bottom
			test = min.x + tW / 2.0f;
			while (test <= max.x - tW / 2.0f) {
				testCol = (int)round((GetUpperLeft().x + test) / tW);
				
				index = topRow * mCols + testCol;
				if (index < mData.size() && mData[index] != 0) {
					if(mSolid) {
						pEventManager->Broadcast(new MapCollisionEvent(pBody, mName, type, Direction::UP, GetUpperLeft().y - ((index / mCols) * tH) - tH));
					}
					collision = true;
				}

				index = bottomRow * mCols + testCol;
				if (index < mData.size() && mData[index] != 0) {
					if (mSolid) {
						pEventManager->Broadcast(new MapCollisionEvent(pBody, mName, type, Direction::DOWN, GetUpperLeft().y - ((index / mCols) * tH)));
					}
					collision = true;
				}

				test += tW;
			}

			// Test left and right
			test = min.y + tH / 2.0f;
			while (test <= max.y - tH / 2.0f) {
				testRow = (int)floor((GetUpperLeft().y - test) / tH);

				index = testRow * mCols + leftCol;
				if (index < mData.size() && mData[index] != 0) {
					if (mSolid) {
						pEventManager->Broadcast(new MapCollisionEvent(pBody, mName, type, Direction::LEFT, mPosition.x + ((index % mCols) * tW) + tW));
					}
					collision = true;
				}

				index = testRow * mCols + rightCol;
				if (index < mData.size() && mData[index] != 0) {
					if (mSolid) {
						pEventManager->Broadcast(new MapCollisionEvent(pBody, mName, type, Direction::RIGHT, mPosition.x + ((index % mCols) * tW)));
					}
					collision = true;
				}
				test += tH;
			}

			if (collision && mDamage > 0.0f) {
				pEventManager->Broadcast(new DamageEvent(nullptr, pBody->GetOwner(), mDamage));
			}
		}
	}
}
