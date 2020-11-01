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
#include "engine/systems/GraphicsManager.h"

#include "engine/systems/graphics/Vertex.h"
#include "engine/systems/graphics/GLTexture.h"
#include "engine/systems/graphics/ShaderProgram.h"

#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/Log.h"

namespace VannoEngine {
	ImageMapLayer::ImageMapLayer()
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
				SetDimensions(static_cast<float>(pTexture->width), static_cast<float>(pTexture->height));
				float w = GetWidth();
				float h = GetHeight();

				Vertex vertexData[4] = {
					 0,  0,   0,   0, 255, 255, 0.0f, 1.0f, // Upper left
					 0, -h, 255,   0,   0, 255, 0.0f, 0.0f, // Bottom Left
					 w, -h,   0, 255,   0, 255, 1.0f, 0.0f, // Bottom right
					 w,  0, 255,   0, 255, 255, 1.0f, 1.0f  // Upper Right
				};

				mpSurface = GraphicsManager::BuildSurface(pTexture, vertexData);

				mpShaderProgram = pResourceManager->LoadShaderProgram("shaders/parallax.shader");
			}
		}
	}

	void ImageMapLayer::Update(double deltaTime) {

	}

	void ImageMapLayer::Draw() {
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0, static_cast<float>(mpSurface->GetHeight()), 0.0f));
		GLuint loc = mpShaderProgram->GetUniformLocation("model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mpSurface->GetTextureId());
		loc = mpShaderProgram->GetUniformLocation("spriteSheet");
		glUniform1i(loc, 0);

		loc = mpShaderProgram->GetUniformLocation("spriteSheetSize");
		glUniform2f(loc, static_cast<float>(mpSurface->GetWidth()), static_cast<float>(mpSurface->GetHeight()));

		loc = mpShaderProgram->GetUniformLocation("spriteSize");
		glUniform2f(loc, static_cast<float>(mpSurface->GetWidth()), static_cast<float>(mpSurface->GetHeight()));

		loc = mpShaderProgram->GetUniformLocation("index");
		glUniform1f(loc, static_cast<GLfloat>(0));

		loc = mpShaderProgram->GetUniformLocation("flipHorizontal");
		glUniform1i(loc, 0);

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
}
