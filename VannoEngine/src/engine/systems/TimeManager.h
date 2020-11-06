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

#include <string>
#include <unordered_map>

namespace VannoEngine {
	class TimeManager
	{
	public:
		static TimeManager* GetInstance();
		~TimeManager();

		double Now();

		unsigned long GetElapsedMillis();

		void StartTimer(std::string name);
		void StopTimer(std::string name);
		double GetTimerSeconds(std::string name);
		unsigned long GetTimerMillis(std::string name);

		void DeleteTimer(std::string name);

	private:
		TimeManager();

		bool HasStartTime(std::string name);
		bool HasEndTime(std::string name);

	private:
		static TimeManager* mpInstance;
		std::chrono::steady_clock::time_point mGameStartTime;

		std::unordered_map<std::string, double> mStartTimes;
		std::unordered_map<std::string, double> mEndTimes;
	};
}