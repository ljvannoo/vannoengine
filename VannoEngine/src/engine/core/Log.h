#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Log.h
Purpose:		Provides logging services to the engine and the game.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-20
*************************************************************************/

#include <memory>

#include "spdlog/spdlog.h"

namespace VannoEngine {
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return sCoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return sClientLogger; }
	
	private:
		static std::shared_ptr<spdlog::logger> sCoreLogger;
		static std::shared_ptr<spdlog::logger> sClientLogger;
	};
}

// Core log macros
#define LOG_CORE_DEBUG(...)	::VannoEngine::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define LOG_CORE_TRACE(...)	::VannoEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_INFO(...)	::VannoEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...)	::VannoEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...)	::VannoEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_CRITICAL(...)	::VannoEngine::Log::GetCoreLogger()->critical(__VA_ARGS__);exit(1)

// Client log macros
#define LOG_DEBUG(...)	::VannoEngine::Log::GetClientLogger()->debug(__VA_ARGS__)
#define LOG_TRACE(...)	::VannoEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)	::VannoEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)	::VannoEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)	::VannoEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)	::VannoEngine::Log::GetClientLogger()->critical(__VA_ARGS__);exit(1)
