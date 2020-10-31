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
#include "engine/systems/graphics/Vertex.h"
#include "engine/systems/graphics/GLTexture.h"

#include "engine/Log.h"

#include <string>

namespace VannoEngine {
	Tileset::Tileset(int tileWidth, int tileHeight) :
		mVboID(0),
		mVaoID(0),
		mIboID(0),
		mpTexture(nullptr),
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
			mpTexture = pResourceManager->LoadTexture(filepath);

			if (mpTexture) {
				float w = mTileWidth;
				float h = mTileHeight;

				Vertex vertexData[6] = {
					 0,  0,   0,   0, 255, 255, 0.0f, 1.0f, // Upper left
					 0, -h, 255,   0,   0, 255, 0.0f, 0.0f, // Bottom Left
					 w, -h,   0, 255,   0, 255, 1.0f, 0.0f, // Bottom right
					 w,  0, 255,   0, 255, 255, 1.0f, 1.0f  // Upper Right
				};

				unsigned int indices[] = {
					0, 1, 2,
					2, 3, 0
				};

				if (mVaoID == 0) {
					glGenVertexArrays(1, &mVaoID);
				}
				glBindVertexArray(mVaoID);

				if (mVboID == 0) {
					glGenBuffers(1, &mVboID);
				}
				glBindBuffer(GL_ARRAY_BUFFER, mVboID);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				if (mIboID == 0) {
					glGenBuffers(1, &mIboID);
				}
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				glBindVertexArray(0);
			}

			if (pData->HasMember("firstgid") && (*pData)["firstgid"].IsInt()) {
				mStartIndex = (*pData)["firstgid"].GetInt();
			}
		}
	}
}
