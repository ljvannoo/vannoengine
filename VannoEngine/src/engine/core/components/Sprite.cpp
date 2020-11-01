/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Sprite.cpp
Purpose:		Implementation of the Sprite class
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-08
*************************************************************************/

#include "engine/Log.h"
#include "Sprite.h"
#include "engine/core/GameObject.h"


#include <cstddef>

#include "engine/systems/graphics/Vertex.h"

#include "engine/systems/graphics/GLTexture.h"
#include "engine/systems/graphics/Surface.h"

#include "engine/systems/ResourceManager.h"
#include "engine/systems/GraphicsManager.h"

namespace VannoEngine {
	Sprite::Sprite(GameObject* owner) :
		GameComponent(owner),
		mpSurface(nullptr),
		mpShaderProgram(nullptr),
		mSheetRows(1),
		mSheetCols(1),
		mFrameOffset(0),
		mAnimationLength(1),
		mFrameIndex(0),
		mElapsedFrameTime(0.0f),
		mFrameTime(0.1f),
		mFlipHorizontal(false),
		mZOrder(0.0f)
	{}

	Sprite::~Sprite() {
		unsigned int bufferId = mpSurface->GetVertexBufferId();
		if (bufferId != 0) {
			glDeleteBuffers(1, &bufferId);
		}

		bufferId = mpSurface->GetIndexBufferId();
		if (bufferId != 0) {
			glDeleteBuffers(1, &bufferId);
		}

		delete mpSurface;
	}

	void Sprite::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
		ResourceManager* pResourceManager = ResourceManager::GetInstance();

		if (pData->HasMember("zOrder") && (*pData)["zOrder"].IsFloat()) {
			mZOrder = (*pData)["zOrder"].GetFloat();
		}

		if (pData->HasMember("texture")) {
			const rapidjson::Value& textureData = (*pData)["texture"];

			if (textureData.HasMember("cols") && textureData["cols"].IsInt()) {
				mSheetCols = textureData["cols"].GetInt();
			}

			if (textureData.HasMember("rows") && textureData["rows"].IsInt()) {
				mSheetRows = textureData["rows"].GetInt();
			}

			if (textureData.HasMember("file") && textureData["file"].IsString()) {
				GLTexture* pTexture = pResourceManager->LoadTexture(textureData["file"].GetString());

				if(pTexture) {
					float w = static_cast<float>(pTexture->width) / static_cast<float>(mSheetCols);
					float hW = w / 2.0f;
					float h = static_cast<float>(pTexture->height) / static_cast<float>(mSheetRows);
					float hH = h / 2.0f;

					Vertex vertexData[4] = {
						-hW,  hH,   0,   0, 255, 255, 0.0f, 1.0f, // Upper left
						-hW, -hH, 255,   0,   0, 255, 0.0f, 0.0f, // Bottom Left
						 hW, -hH,   0, 255,   0, 255, 1.0f, 0.0f, // Bottom right
						 hW,  hH, 255,   0, 255, 255, 1.0f, 1.0f  // Upper Right
					};

					mpSurface = GraphicsManager::BuildSurface(pTexture, vertexData);
				}
			}

			if (textureData.HasMember("flipHorizontal") && textureData["flipHorizontal"].IsBool()) {
				mFlipHorizontal = textureData["flipHorizontal"].GetBool();
			}

			if (textureData.HasMember("defaultAnimation")) {
				const rapidjson::Value& animationData = textureData["defaultAnimation"];

				if (animationData.HasMember("frameOffset") && animationData["frameOffset"].IsInt()) {
					mFrameOffset = animationData["frameOffset"].GetInt();
				}

				if (animationData.HasMember("frameCount") && animationData["frameCount"].IsInt()) {
					mAnimationLength = animationData["frameCount"].GetInt();
				}

				if (animationData.HasMember("frameDuration") && animationData["frameDuration"].IsFloat()) {
					mFrameTime = animationData["frameDuration"].GetFloat();
				}
			}
		}

		ShaderProgram* pShaderProgram = nullptr;
		if (pData->HasMember("shaderProgram") && (*pData)["shaderProgram"].IsString()) {
			mpShaderProgram = pResourceManager->LoadShaderProgram((*pData)["shaderProgram"].GetString());
		}

	}

	void Sprite::Update(double deltaTime) {
		mElapsedFrameTime += deltaTime;

		if (mElapsedFrameTime >= mFrameTime) {
			mFrameIndex = (mFrameIndex + 1) % mAnimationLength;
			mElapsedFrameTime = 0.0f;
		}
	}

	void Sprite::Draw() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mpSurface->GetTextureId());
		GLuint loc = mpShaderProgram->GetUniformLocation("spriteSheet");
		glUniform1i(loc, 0);

		loc = mpShaderProgram->GetUniformLocation("spriteSheetSize");
		glUniform2f(loc, static_cast<float>(mpSurface->GetWidth()), static_cast<float>(mpSurface->GetHeight()));

		loc = mpShaderProgram->GetUniformLocation("spriteSize");
		glUniform2f(loc, static_cast<float>(mpSurface->GetWidth() / mSheetCols), static_cast<float>(mpSurface->GetHeight() / mSheetRows));

		loc = mpShaderProgram->GetUniformLocation("index");
		glUniform1f(loc, static_cast<GLfloat>(mFrameOffset + mFrameIndex));

		loc = mpShaderProgram->GetUniformLocation("flipHorizontal");
		glUniform1i(loc, mFlipHorizontal ? 1 : 0);

		glBindVertexArray(mpSurface->GetVertexArrayId());
		glBindBuffer(GL_ARRAY_BUFFER, mpSurface->GetVertexBufferId());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mpSurface->GetIndexBufferId());

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

	ShaderProgram* Sprite::GetShaderProgram() {
		return mpShaderProgram;
	}

	float Sprite::GetHeight() {
		return (float)mpSurface->GetHeight() / (float)mSheetRows;
	}

	float Sprite::GetWidth() {
		return (float)mpSurface->GetWidth() / (float)mSheetCols;
	}
}