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
#include "engine/systems/graphics/ShaderProgram.h"
#include "engine/systems/graphics/Vertex.h"

#include "engine/Log.h"

#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>

namespace VannoEngine {
	TileMapLayer::TileMapLayer(int tileWidth, int tileHeight) :
		MapLayer(),
		mTileWidth(tileWidth),
		mTileHeight(tileHeight),
		mpTilesets(nullptr)
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

		ResourceManager* pResourceManager = ResourceManager::GetInstance();
		mpShaderProgram = pResourceManager->LoadShaderProgram("shaders/tileset.shader");
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
					glm::vec2 topLeft(0.0f, GetHeight());
					int col = i % ((int)GetWidth() / mTileWidth);
					int row = i / ((int)GetWidth() / mTileWidth);
					float x = topLeft.x + static_cast<float>(col * pTileset->GetTileWidth());
					float y = topLeft.y - static_cast<float>((row+1) * pTileset->GetTileHeight());
					glm::mat4 t(1.0f);
					t = glm::translate(t, glm::vec3(0.0f, static_cast<float>(pTileset->GetTileWidth()), 0.0f));
					t = glm::translate(t, glm::vec3(x, y, 0.0f));

					GLuint loc = mpShaderProgram->GetUniformLocation("model");
					glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(t));

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, pTileset->GetTextureId());
					loc = mpShaderProgram->GetUniformLocation("tileset");
					glUniform1i(loc, 0);

					loc = mpShaderProgram->GetUniformLocation("tilesetSize");
					glUniform2f(loc, static_cast<float>(pTileset->GetWidth()), static_cast<float>(pTileset->GetHeight()));

					loc = mpShaderProgram->GetUniformLocation("tileSize");
					glUniform2f(loc, static_cast<float>(pTileset->GetTileWidth()), static_cast<float>(pTileset->GetTileHeight()));

					float index = static_cast<GLfloat>(tileId);
					loc = mpShaderProgram->GetUniformLocation("index");
					glUniform1f(loc, index);

					glBindVertexArray(pTileset->GetVertexArrayId());
					glBindBuffer(GL_ARRAY_BUFFER, pTileset->GetVertexBufferId());
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pTileset->GetIndexBufferId());

					glEnableVertexAttribArray(0);

					// Position
					glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

					// Color
					glEnableVertexAttribArray(1);
					glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

					// Texture coordinates
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

					glDisableVertexAttribArray(0);
					glDisableVertexAttribArray(1);
					glDisableVertexAttribArray(2);

					// Unbind the buffer
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glBindVertexArray(0);

					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
		}
	}
}