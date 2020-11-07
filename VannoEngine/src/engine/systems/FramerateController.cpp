/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		FramerateController.cpp
Purpose:		Implementation of the framerate manager
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-06
*************************************************************************/
#include "FramerateController.h"
#include "TimeManager.h"

#include "engine/core/Log.h"

#include <thread>
namespace VannoEngine {
	FramerateController* FramerateController::mpInstance = nullptr;

	FramerateController::~FramerateController() {
		if (mpInstance) {
			delete mpInstance;
		}
	}

	FramerateController* FramerateController::GetInstance() {
		if (!mpInstance) {
			mpInstance = new FramerateController;
		}

		return mpInstance;
	}

	void FramerateController::SetMaxFramerate(int maxFrameRate) {
		mFrameRateCap = maxFrameRate;
	}

	void FramerateController::StartFrame() {
		mFrameStartTime = mpTime->Now();
	}

	void FramerateController::EndFrame() {
		TimeManager::GetInstance()->StartTimer("framewaste");
		
		// If we have a lot of time left in the frame, try sleeping for a millisecond.  Sleeping is not
		// very precise because there is no guarantee when the CPU will give back control, but if we
		// leave a large enough margin, framerate is mostly maintained and the load on the CPU is
		// reduced by half or more when compared to spin-locking only.
		mFrameEndTime = mpTime->Now();
		mDeltaTime = mFrameEndTime - mFrameStartTime;
		long timeLeft = (long)((1.0 / (double)mFrameRateCap - mDeltaTime) * 1000);
		if (timeLeft >= 4) {
			std::this_thread::sleep_for(std::chrono::milliseconds(timeLeft/2));
		}
		

		// spin lock for the remainder of the time
		do {
			mFrameEndTime = mpTime->Now();
			mDeltaTime = mFrameEndTime - mFrameStartTime;	
		} while (mFrameRateCap != 0 && mDeltaTime < (1.0 / mFrameRateCap));
		TimeManager::GetInstance()->StopTimer("framewaste");
	}

	double FramerateController::GetDeltaTime() {
		return mDeltaTime;
	}

	double FramerateController::GetFPS() {
		return 1.0 / mDeltaTime;
	}
}
