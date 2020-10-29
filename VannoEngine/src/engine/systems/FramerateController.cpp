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

		do {
			mFrameEndTime = mpTime->Now();
			mDeltaTime = mFrameEndTime - mFrameStartTime;

			// I tried to use sleep here rather than spin-locking in an attempt to give some CPU time back to the OS. In practice,
			// 500 nanoseconds doesn't change much, and anything greater caused the OS to schedule tasks that had too much affect
			// on the consistency of the framerate.  So, for now, I'm just going to spin-lock.  This may be an optimization for the future.

			// std::this_thread::sleep_for(std::chrono::nanoseconds(500));
		} while (mFrameRateCap != 0 && mDeltaTime < (1.0 / mFrameRateCap));

	}

	double FramerateController::GetDeltaTime() {
		return mDeltaTime;
	}

	double FramerateController::GetFPS() {
		return 1.0 / mDeltaTime;
	}
}
