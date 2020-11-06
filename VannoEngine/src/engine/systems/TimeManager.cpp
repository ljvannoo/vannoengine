/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		TimeManager.cpp
Purpose:		Implementation of the TimeManager
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-13
*************************************************************************/
#include "TimeManager.h"

namespace VannoEngine {
	TimeManager* TimeManager::mpInstance = nullptr;

	TimeManager* TimeManager::GetInstance() {
		if (!mpInstance) {
			mpInstance = new TimeManager;
		}

		return mpInstance;
	}

	TimeManager::~TimeManager() {
		if (mpInstance) {
			delete mpInstance;
		}
	}

	TimeManager::TimeManager() {
		mGameStartTime = std::chrono::high_resolution_clock::now();
	}

	bool TimeManager::HasStartTime(std::string name) {
		if (mStartTimes.find(name) == mStartTimes.end()) {
			return false;
		}
		return true;
	}

	bool TimeManager::HasEndTime(std::string name) {
		if (mEndTimes.find(name) == mEndTimes.end()) {
			return false;
		}
		return true;
	}

	double TimeManager::Now() {
		auto now = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::microseconds>(now - mGameStartTime).count() / 1000000.0;
	}


	unsigned long TimeManager::GetElapsedMillis() {
		return static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - mGameStartTime).count());
	}

	void TimeManager::StartTimer(std::string name) {
		mStartTimes[name] = Now();
		if (HasEndTime(name)) {
			mEndTimes.erase(name);
		}
	}

	void TimeManager::StopTimer(std::string name) {
		if (HasStartTime(name)) {
			mEndTimes[name] = Now();
		}
	}

	void TimeManager::DeleteTimer(std::string name) {
		if (HasStartTime(name)) {
			mStartTimes.erase(name);
		}

		if (HasEndTime(name)) {
			mEndTimes.erase(name);
		}
	}

	double TimeManager::GetTimerSeconds(std::string name) {
		if (HasStartTime(name)) {
			double startTime = mStartTimes[name];

			double endTime = Now();
			if (HasEndTime(name)) {
				endTime = mEndTimes[name];
			}

			return endTime - startTime;
		}

		return 0.0;
	}
	unsigned long TimeManager::GetTimerMillis(std::string name) {
		return (long)(GetTimerSeconds(name) * 1000.0);
	}
}