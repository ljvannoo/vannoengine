#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		FramerateManager.h
Purpose:		Provides functionality for monitoring and capping the game's
				frame rate.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-06
*************************************************************************/

#include "TimeManager.h"

namespace VannoEngine {
	class ENGINE_API FramerateController
	{
	public:
	public:
		static FramerateController* GetInstance();
		~FramerateController();

		void SetMaxFramerate(int maxFrameRate);

		void StartFrame();
		void EndFrame();

		double GetDeltaTime();
		double GetFPS();

		// Number of milliseconds since the game started
		long GetElapsedMillis();
	private:
		FramerateController() :
			mDeltaTime(0.0),
			mFrameRateCap(60),
			mFrameStartTime(0.0),
			mFrameEndTime(0.0),
			mpTime(nullptr)
		{
			mpTime = TimeManager::GetInstance();
		};
	private:
		int mFrameRateCap;
		static FramerateController* mpInstance;
		double mFrameStartTime;
		double mFrameEndTime;
		double mDeltaTime;

		TimeManager* mpTime;
	};
}