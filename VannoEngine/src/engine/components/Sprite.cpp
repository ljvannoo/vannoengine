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

#include "engine/core/Log.h"

#include "engine/components/Transform.h"
#include "engine/components/Animator.h"

#include "engine/systems/objects/GameObject.h"

#include "engine/systems/graphics/Vertex.h"
#include "engine/systems/graphics/GLTexture.h"
#include "engine/systems/graphics/Surface.h"

#include "engine/systems/ResourceManager.h"
#include "engine/systems/graphics/GraphicsManager.h"

#include "engine/systems/ConfigurationManager.h"

#include <cstddef>

namespace VannoEngine {
	Sprite::Sprite(GameObject* owner) :
		GameComponent(owner),
		mpSurface(nullptr),
		mSheetRows(1),
		mSheetCols(1),
		mFlipHorizontal(false),
		mZOrder(0.0f),
		mUiMode{ false }
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

		if (pData->HasMember("uiMode") && (*pData)["uiMode"].IsBool()) {
			mUiMode = (*pData)["uiMode"].GetBool();
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
						-hW,  hH, 255,   0, 255, 255, 0.0f, 1.0f, // Upper left
						-hW, -hH, 255,   0,   0, 255, 0.0f, 0.0f, // Bottom Left
						 hW, -hH,   0, 255,   0, 255, 1.0f, 0.0f, // Bottom right
						 hW,  hH,   0,   0, 255, 255, 1.0f, 1.0f  // Upper Right
					};

					mpSurface = GraphicsManager::BuildSurface(pTexture, vertexData);
				}
			}

			if (textureData.HasMember("flipHorizontal") && textureData["flipHorizontal"].IsBool()) {
				mFlipHorizontal = textureData["flipHorizontal"].GetBool();
			}
		}
	}

	void Sprite::Update(double deltaTime) {
		
	}

	void Sprite::Draw() {
		GraphicsManager* pGraphicsManager = GraphicsManager::GetInstance();

		float spriteSheetWidth = static_cast<float>(mpSurface->GetWidth());
		float spriteSheetHeight = static_cast<float>(mpSurface->GetHeight());
		float spriteWidth = static_cast<float>(mpSurface->GetWidth() / mSheetCols);
		float spriteHeight = static_cast<float>(mpSurface->GetHeight() / mSheetRows);
		
		int spriteIndex = 0;
		Animator* pAnimator = static_cast<Animator*>(GetOwner()->GetComponent(ANIMATOR_COMPONENT));
		if(pAnimator) {
			spriteIndex = pAnimator->GetFrameIndex();
		}

		glm::mat4 model(1.0f);
		
		Transform* pTransform = static_cast<Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));
		if(pTransform) {
			glm::mat4 t = pTransform->GetTranslationMatrix();
			glm::mat4 r = pTransform->GetRotationMatrix();
			glm::mat4 s = pTransform->GetScaleMatrix();
			model = t * r * s;

			int cols = (int)(spriteSheetWidth / spriteWidth);
			int col = spriteIndex % cols;
			int row = spriteIndex / cols;
			pGraphicsManager->Render(mpSurface, &model, spriteSheetWidth, spriteSheetHeight, spriteWidth, spriteHeight, col, row, mFlipHorizontal, 0, mUiMode);
		}
	}

	float Sprite::GetHeight() {
		return (float)mpSurface->GetHeight() / (float)mSheetRows;
	}

	float Sprite::GetWidth() {
		return (float)mpSurface->GetWidth() / (float)mSheetCols;
	}
}