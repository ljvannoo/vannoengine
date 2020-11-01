/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Tileset.cpp
Purpose:		Implements a tileset
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-10
*************************************************************************/
#include "Tileset.h"

#include "engine/systems/ResourceManager.h"

#include "engine/systems/graphics/GraphicsManager.h"
#include "engine/systems/graphics/Vertex.h"
#include "engine/systems/graphics/GLTexture.h"
#include "engine/systems/graphics/Surface.h"

#include "engine/core/Log.h"

#include <string>

namespace VannoEngine {
	Tileset::Tileset(int tileWidth, int tileHeight) :
		mpSurface(nullptr),
		mStartIndex(1),
		mTileWidth(tileWidth),
		mTileHeight(tileHeight)
	{ }

	Tileset::~Tileset() {

	}

	void Tileset::LoadData(const rapidjson::Value* pData) {
		ResourceManager* pResourceManager = ResourceManager::GetInstance();

		if (pData->HasMember("source") && (*pData)["source"].IsString()) {
			std::string filepath = (*pData)["source"].GetString();
			filepath.replace(filepath.end() - 3, filepath.end(), "png");

			LOG_CORE_INFO("Loading tileset from '{0}'", filepath);
			GLTexture* pTexture = pResourceManager->LoadTexture(filepath);

			if (pTexture) {
				float w = (float)mTileWidth;
				float h = (float)mTileHeight;

				Vertex vertexData[4] = {
					 0,  0,   0,   0, 255, 255, 0.0f, 1.0f, // Upper left
					 0, -h, 255,   0,   0, 255, 0.0f, 0.0f, // Bottom Left
					 w, -h,   0, 255,   0, 255, 1.0f, 0.0f, // Bottom right
					 w,  0, 255,   0, 255, 255, 1.0f, 1.0f  // Upper Right
				};
				mpSurface = GraphicsManager::BuildSurface(pTexture, vertexData);
			}

			if (pData->HasMember("firstgid") && (*pData)["firstgid"].IsInt()) {
				mStartIndex = (*pData)["firstgid"].GetInt();
			}
		}
	}

	float Tileset::GetWidth() {
		return (float)mpSurface->GetWidth();
	}

	float Tileset::GetHeight() {
		return (float)mpSurface->GetHeight();
	}

	unsigned int Tileset::GetTextureId() {
		return mpSurface->GetTextureId();
	}

	unsigned int Tileset::GetVertexBufferId() {
		return mpSurface->GetVertexBufferId(); 
	}

	unsigned int Tileset::GetVertexArrayId() {
		return mpSurface->GetVertexArrayId();
	}

	unsigned int Tileset::GetIndexBufferId() {
		return mpSurface->GetIndexBufferId(); 
	}
}
