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

#include "engine/systems/graphics/Vertex.h"
#include "engine/systems/graphics/GLTexture.h"
#include "engine/systems/graphics/ShaderProgram.h"

#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/Log.h"

namespace VannoEngine {
	ImageMapLayer::ImageMapLayer() :
		mName(""),
		mVboID(0),
		mVaoID(0),
		mIboID(0),
		mpTexture(nullptr)
	{ }

	ImageMapLayer::~ImageMapLayer() {

	}

	void ImageMapLayer::LoadData(const rapidjson::Value* pData) {
		if (pData->HasMember("name") && (*pData)["name"].IsString()) {
			mName = (*pData)["name"].GetString();
			LOG_CORE_INFO("Loading image map layer '{0}'", mName);
		}

		if (pData->HasMember("image") && (*pData)["image"].IsString()) {
			ResourceManager* pResourceManager = ResourceManager::GetInstance();
			std::string filepath = (*pData)["image"].GetString();

			mpTexture = pResourceManager->LoadTexture(filepath);
			
			if (mpTexture) {
				float w = static_cast<float>(mpTexture->width);
				float h = static_cast<float>(mpTexture->height);

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

				mpShaderProgram = pResourceManager->LoadShaderProgram("shaders/parallax.shader");
			}
		}
	}

	void ImageMapLayer::Draw() {
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0, static_cast<float>(mpTexture->height), 0.0f));
		GLuint loc = mpShaderProgram->GetUniformLocation("model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mpTexture->id);
		loc = mpShaderProgram->GetUniformLocation("spriteSheet");
		glUniform1i(loc, 0);

		loc = mpShaderProgram->GetUniformLocation("spriteSheetSize");
		glUniform2f(loc, static_cast<float>(mpTexture->width), static_cast<float>(mpTexture->height));

		loc = mpShaderProgram->GetUniformLocation("spriteSize");
		glUniform2f(loc, static_cast<float>(mpTexture->width), static_cast<float>(mpTexture->height));

		loc = mpShaderProgram->GetUniformLocation("index");
		glUniform1f(loc, static_cast<GLfloat>(0));

		loc = mpShaderProgram->GetUniformLocation("flipHorizontal");
		glUniform1i(loc, 0);

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
}
