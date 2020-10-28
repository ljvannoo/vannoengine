#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		TimeManager.h
Purpose:		Utility class to handle everything time releated
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-13
*************************************************************************/

#include <chrono>

#include "engine/core.h"

namespace VannoEngine {
	class ENGINE_API TimeManager
	{
	public:
		static TimeManager* GetInstance();
		~TimeManager();

		double Now();

		long GetElapsedMillis();

	private:
		TimeManager();

	private:
		static TimeManager* mpInstance;
		std::chrono::steady_clock::time_point mGameStartTime;
	};
}