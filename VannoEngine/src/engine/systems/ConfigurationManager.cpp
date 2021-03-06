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

#include "ConfigurationManager.h"

#include "engine/core/Log.h"

#include "engine/systems/ResourceManager.h"

#include <rapidjson/pointer.h>

namespace VannoEngine {
	ConfigurationManager* ConfigurationManager::mpInstance = nullptr;

	ConfigurationManager* ConfigurationManager::GetInstance() {
		if (!mpInstance) {
			mpInstance = new ConfigurationManager;
		}

		return mpInstance;
	}

	ConfigurationManager::ConfigurationManager() {

	}

	ConfigurationManager::~ConfigurationManager() {
		if (mpInstance) {
			delete mpInstance;
		}
	}
	 
	void ConfigurationManager::Init(std::string relativePath) {
		ResourceManager* pResourceManager = ResourceManager::GetInstance();
		mpConfigData = pResourceManager->LoadJsonData(relativePath);
	}

	std::string ConfigurationManager::GetString(std::string key) {
		rapidjson::Value* value = GetValue(key);

		if (value && value->IsString()) {
			return value->GetString();
		}
		return std::string();
	}

	int ConfigurationManager::GetInt(std::string key) {
		rapidjson::Value* value = GetValue(key);

		if (value && value->IsNumber()) {
			return value->GetInt();
		}
		return 0;
	}

	float ConfigurationManager::GetFloat(std::string key) {
		rapidjson::Value* value = GetValue(key);

		if (value && value->IsNumber()) {
			return value->GetFloat();
		}
		return 0.0f;
	}

	void ConfigurationManager::SetBool(std::string key, bool value) {
		rapidjson::Pointer(key.c_str()).Set(*mpConfigData, value);
		if (HasValue(key)) {
			mValueCache.erase(key);
		}
	}

	bool ConfigurationManager::GetBool(std::string key) {
		rapidjson::Value* value = GetValue(key);

		if (value && value->IsBool()) {
			return value->GetBool();
		}
		return false;
	}

	void ConfigurationManager::ToggleBool(std::string key) {
		SetBool(key, !GetBool(key));
	}

	rapidjson::Value* ConfigurationManager::GetValue(std::string key) {
		rapidjson::Value* value = nullptr;
		if (HasValue(key)) {
			value = mValueCache[key];
		}
		else {
			rapidjson::Pointer ptr = rapidjson::Pointer(key.c_str());
			if (ptr.IsValid()) {
				value = ptr.Get(*mpConfigData);
				mValueCache[key] = value;
			}
		}
		return value;
	}

	bool ConfigurationManager::HasValue(std::string key) {
		return mValueCache.find(key) != mValueCache.end();
	}
}