/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		ImageMapLayer.cpp
Purpose:		Implementation of a ImageMapLayer
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-29
*************************************************************************/
#include "ImageMapLayer.h"

#include "engine/systems/ResourceManager.h"

#include "engine/systems/levels/LevelManager.h"
#include "engine/systems/levels/Level.h"

#include "engine/components/Camera.h"

#include "engine/systems/graphics/GraphicsManager.h"
#include "engine/systems/graphics/Vertex.h"
#include "engine/systems/graphics/GLTexture.h"
#include "engine/systems/graphics/ShaderProgram.h"

#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "engine/core/Log.h"

namespace VannoEngine {
	ImageMapLayer::ImageMapLayer() :
		mpSurface{ nullptr },
		mSpeed{ 0.0f }
	{ }

	ImageMapLayer::~ImageMapLayer() {
		delete mpSurface;
	}

	void ImageMapLayer::LoadData(const rapidjson::Value* pData) {
		if (pData->HasMember("name") && (*pData)["name"].IsString()) {
			mName = (*pData)["name"].GetString();
			LOG_CORE_INFO("Loading image map layer '{0}'", mName);
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

		if (pData->HasMember("image") && (*pData)["image"].IsString()) {
			ResourceManager* pResourceManager = ResourceManager::GetInstance();
			std::string filepath = (*pData)["image"].GetString();

			GLTexture* pTexture = pResourceManager->LoadTexture(filepath);
			
			if (pTexture) {
				SetDimensions((float)pTexture->width, (float)pTexture->height);
				float w = GetWidth();
				float h = GetHeight();

				Vertex vertexData[4] = {
					 0,  0,   0,   0, 255, 255, 0.0f, 1.0f, // Upper left
					 0, -h, 255,   0,   0, 255, 0.0f, 0.0f, // Bottom Left
					 w, -h,   0, 255,   0, 255, 1.0f, 0.0f, // Bottom right
					 w,  0, 255,   0, 255, 255, 1.0f, 1.0f  // Upper Right
				};

				mpSurface = GraphicsManager::BuildSurface(pTexture, vertexData);
			}
		}

		if (pData->HasMember("properties") && (*pData)["properties"].IsArray()) {
			const rapidjson::Value& properties = (*pData)["properties"];

			for (rapidjson::SizeType i = 0; i < properties.Size(); i++) {
				const rapidjson::Value& property = properties[i];

				if (property.HasMember("name") && property["name"].IsString() && std::string(property["name"].GetString()) == "speed" &&
					property.HasMember("value") && property["value"].IsNumber()) {
					mSpeed = property["value"].GetFloat();
				}
			}
		}
	}

	void ImageMapLayer::Update(double deltaTime) {

	}

	void ImageMapLayer::Draw() {
		GraphicsManager* pGraphicsManager = GraphicsManager::GetInstance();
		Level* pLevel = LevelManager::GetInstance()->GetCurrentLevel();
		Camera* pCamera = LevelManager::GetInstance()->GetCamera();

		float imageWidth = (float)mpSurface->GetWidth();
		float imageHeight = (float)mpSurface->GetHeight();

		float screenHalfHeight = pCamera->GetScreenHeight() / 2.0f;
		float cameraY = pCamera->GetPosition().y;

		float offsetX = 0.0f;
		offsetX = pCamera->GetPosition().x * mSpeed;
		for (float x = offsetX-imageWidth; x < pLevel->GetWidth(); x += imageWidth) {
			glm::mat4 model(1.0f);
			glm::vec3 position = glm::vec3(x, cameraY - screenHalfHeight + imageHeight, 0.0f);
			model = glm::translate(model, position);

			// TODO Only render if image is on the screen
			pGraphicsManager->Render(mpSurface, &model, imageWidth, imageHeight, imageWidth, imageHeight, 0, false);
		}
	}
}
