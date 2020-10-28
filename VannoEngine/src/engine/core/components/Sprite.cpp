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

#include "Sprite.h"
#include "engine/core/GameObject.h"

#include <iostream>
#include <cstddef>

#include "engine/systems/graphics/Vertex.h"

#include "engine/systems/graphics/GLTexture.h"
#include "engine/systems/ResourceManager.h"

namespace VannoEngine {
	Sprite::Sprite(GameObject* owner) :
		GameComponent(owner),
		mVboID(0),
		mVaoID(0),
		mIboID(0),
		mpTexture(nullptr),
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
		if (mVboID != 0) {
			glDeleteBuffers(1, &mVboID);
		}
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
				mpTexture = pResourceManager->LoadTexture(textureData["file"].GetString());

				if (mpTexture) {
					float w = static_cast<float>(mpTexture->width) / static_cast<float>(mSheetCols);
					float hW = w / 2.0f;
					float h = static_cast<float>(mpTexture->height) / static_cast<float>(mSheetRows);
					float hH = h / 2.0f;

					//Vertex vertexData[6] = {
					//	-hW,  hH,   0,   0, 255, 255, 0.0f, 1.0f, // Upper left
					//	-hW, -hH, 255,   0,   0, 255, 0.0f, 0.0f, // Bottom Left
					//	 hW,  hH, 255,   0, 255, 255, 1.0f, 1.0f, // Upper Right
					//	 hW,  hH, 255,   0, 255, 255, 1.0f, 1.0f, // Upper Right
					//	-hW, -hH, 255,   0,   0, 255, 0.0f, 0.0f, // Bottom Left
					//	 hW, -hH,   0, 255,   0, 255, 1.0f, 0.0f  // Bottom right
					//};

					Vertex vertexData[6] = {
						-hW,  hH,   0,   0, 255, 255, 0.0f, 1.0f, // Upper left
						-hW, -hH, 255,   0,   0, 255, 0.0f, 0.0f, // Bottom Left
						 hW, -hH,   0, 255,   0, 255, 1.0f, 0.0f, // Bottom right
						 hW,  hH, 255,   0, 255, 255, 1.0f, 1.0f  // Upper Right
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

				if (animationData.HasMember("frameDuration") && animationData["frameDuration"].GetFloat()) {
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
		glBindTexture(GL_TEXTURE_2D, mpTexture->id);
		GLuint loc = mpShaderProgram->GetUniformLocation("spriteSheet");
		glUniform1i(loc, 0);

		loc = mpShaderProgram->GetUniformLocation("spriteSheetSize");
		glUniform2f(loc, static_cast<float>(mpTexture->width), static_cast<float>(mpTexture->height));

		loc = mpShaderProgram->GetUniformLocation("spriteSize");
		glUniform2f(loc, static_cast<float>(mpTexture->width / mSheetCols), static_cast<float>(mpTexture->height / mSheetRows));

		loc = mpShaderProgram->GetUniformLocation("index");
		glUniform1f(loc, static_cast<GLfloat>(mFrameOffset + mFrameIndex));

		loc = mpShaderProgram->GetUniformLocation("flipHorizontal");
		glUniform1i(loc, mFlipHorizontal ? 1 : 0);

		glBindVertexArray(mVaoID);
		glBindBuffer(GL_ARRAY_BUFFER, mVboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboID);

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
}