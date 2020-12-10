/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Log.cpp
Purpose:		Provides logging services to the engine and the game.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-20
*************************************************************************/


#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace VannoEngine {
	std::shared_ptr<spdlog::logger> Log::sCoreLogger;
	std::shared_ptr<spdlog::logger> Log::sClientLogger;

	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");

		sCoreLogger = spdlog::stdout_color_mt("Engine");
		sCoreLogger->set_level(spdlog::level::trace);
		sClientLogger = spdlog::stdout_color_mt("Game");
		sClientLogger->set_level(spdlog::level::trace);
	}
}
