/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		ResourceManager.cpp
Purpose:		Implementation of the ResourceManager
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-10
*************************************************************************/
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ResourceManager.h"
#include "graphics/ShaderProgram.h"

#include "engine/core/Log.h"

#include <fstream>
#include <time.h>
#include <stdlib.h>

namespace VannoEngine {
	ResourceManager* ResourceManager::mpInstance = nullptr;
	ResourceManager::ResourceManager() 
	{
		srand(static_cast<unsigned int>(time(0)));
	}

	ResourceManager::~ResourceManager() {
		// Delete cached textures
		for (auto pair : mTextures) {
			stbi_image_free(pair.second->pData);
			free(pair.second);
		}
		mTextures.clear();

		// Delete cached text data
		for (auto pair : mTextData) {
			delete pair.second;
		}
		mTextData.clear();

		// Delete JSON data
		for (auto pair : mJsonData) {
			delete pair.second;
		}
		mJsonData.clear();

		// Delete Shader Programs
		for (auto pair : mShaderPrograms) {
			delete pair.second;
		}
		mShaderPrograms.clear();
	}

	ResourceManager* ResourceManager::GetInstance() {
		if (!mpInstance) {
			mpInstance = new ResourceManager;
		}

		return mpInstance;
	}

	GLTexture* ResourceManager::LoadTexture(const std::string& relativePath) {
		std::string filePath("resources\\" + relativePath);

		GLTexture* pTexture = mTextures[filePath];

		if (!pTexture) {
			pTexture = new GLTexture;
			int nrChannels;
			stbi_set_flip_vertically_on_load(true);
			pTexture->pData = stbi_load(filePath.c_str(), &pTexture->width, &pTexture->height, &nrChannels, 0);

			if (pTexture->pData) {
				glGenTextures(1, &pTexture->id);
				glBindTexture(GL_TEXTURE_2D, pTexture->id);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pTexture->width, pTexture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pTexture->pData);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

				glGenerateMipmap(GL_TEXTURE_2D);

				glBindTexture(GL_TEXTURE_2D, 0);

				mTextures[filePath] = pTexture;
			}
			else {
				LOG_CORE_ERROR("Failed to load texture {0}", relativePath);
				delete pTexture;
			}
		}

		return pTexture;
	}

	std::string* ResourceManager::LoadFile(const std::string& filePath) {
		std::string* pData = new std::string;
		std::ifstream file(filePath);
		if (file.fail()) {
			LOG_CORE_CRITICAL("Failed to open text file from " + filePath);
		}

		std::string line;
		while (std::getline(file, line)) {
			*pData += line + "\n";
		}
		file.close();

		return pData;
	}

	std::string* ResourceManager::LoadTextData(const std::string& relativePath) {
		std::string filePath("resources\\" + relativePath);

		std::string* pTextData = mTextData[filePath];

		if (!pTextData) {
			pTextData = LoadFile(filePath);

			mTextData[filePath] = pTextData;
		}

		return pTextData;
	}

	rapidjson::Document* ResourceManager::LoadJsonData(const std::string& relativePath) {
		std::string filePath("resources\\" + relativePath);

		rapidjson::Document* pDocument = mJsonData[filePath];

		if (!pDocument) {
			std::string* src = LoadFile(filePath);

			pDocument = new rapidjson::Document;
			pDocument->Parse(src->c_str());

			delete src;

			mJsonData[filePath] = pDocument;
		}
		return pDocument;
	}

	ShaderProgram* ResourceManager::LoadShaderProgram(const std::string& relativePath) {
		std::string filePath("resources\\" + relativePath);

		ShaderProgram* pShaderProgram = mShaderPrograms[filePath];

		if (!pShaderProgram) {
			std::string vertexShaderSrc;
			std::string fragmentShaderSrc;
			std::string* pCurrentSrc = nullptr;

			std::ifstream file(filePath);
			if (file.fail()) {
				LOG_CORE_CRITICAL("Failed to open shader source from " + filePath);
			}

			std::string line;
			while (std::getline(file, line)) {
				if (line == "#BEGIN_VERTEX_SHADER") {
					pCurrentSrc = &vertexShaderSrc;
				}
				else if (line == "#BEGIN_FRAGMENT_SHADER") {
					pCurrentSrc = &fragmentShaderSrc;
				}
				else if (pCurrentSrc != nullptr) {
					*pCurrentSrc += line + "\n";
				}
			}
			file.close();

			pShaderProgram = new ShaderProgram();
			pShaderProgram->Init(vertexShaderSrc, fragmentShaderSrc);

			mShaderPrograms[filePath] = pShaderProgram;
		}

		return pShaderProgram;
	}

	float ResourceManager::GenerateRandomNumber(float min, float max, float precision) {
		float actualMin = (float)min * precision;
		float actualMax = (float)max * precision;
		int result = (rand() % static_cast<int>((actualMax - actualMin)) + actualMin);
		return (float)result / precision;
	}
}