#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		ConfigurationManager.h
Purpose:		Managers general application configuration
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-13
*************************************************************************/
#include <rapidjson/document.h>

#include <string>
#include <unordered_map>

namespace VannoEngine {
	class ConfigurationManager
	{
	public:
		static ConfigurationManager* GetInstance();
		~ConfigurationManager();

		void Init(std::string relativePath);

		std::string GetString(std::string key);
		int GetInt(std::string key);
		float GetFloat(std::string key);

		void SetBool(std::string key, bool value);
		bool GetBool(std::string key);
		void ToggleBool(std::string key);

	private:
		ConfigurationManager();

		bool HasValue(std::string key);
		rapidjson::Value* GetValue(std::string key);


	private:
		static ConfigurationManager* mpInstance;

		rapidjson::Document* mpConfigData;

		std::unordered_map<std::string, rapidjson::Value*> mValueCache;
	};
}
