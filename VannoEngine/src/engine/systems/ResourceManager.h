#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		GLSLProgram.h
Purpose:		Provides functionality for loading and caching resources.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-10
*************************************************************************/
#include <string>
#include <vector>
#include <unordered_map>

#include <rapidjson/document.h>

#include "engine/systems/graphics/GLTexture.h"

namespace VannoEngine {
	class ShaderProgram;

	class ResourceManager
	{
	public:
	public:
		static ResourceManager* GetInstance();
		~ResourceManager();

		GLTexture* LoadTexture(const std::string& relativePath);

		std::string* LoadTextData(const std::string& relativePath);
		rapidjson::Document* LoadJsonData(const std::string& relativePath);
		ShaderProgram* LoadShaderProgram(const std::string& relativePath);

		float GenerateRandomNumber(float min, float max, float precision);
	private:
		ResourceManager();
		std::string* LoadFile(const std::string& relativePath);
	private:
		static ResourceManager* mpInstance;
		std::unordered_map<std::string, GLTexture* > mTextures;
		std::unordered_map<std::string, std::string* > mTextData;
		std::unordered_map<std::string, rapidjson::Document* > mJsonData;
		std::unordered_map<std::string, ShaderProgram* > mShaderPrograms;
	};
}