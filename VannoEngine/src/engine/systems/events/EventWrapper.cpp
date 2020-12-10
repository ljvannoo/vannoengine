/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		EventWrapper.cpp
Purpose:		Wraps events and adds a time when the event should be triggered
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-23
*************************************************************************/

#include "EventWrapper.h"

#include "Event.h"

namespace VannoEngine {
	EventWrapper::EventWrapper(double time, Event* message) :
		mTime{ time },
		mEvent{ message }
	{ }

	EventWrapper::~EventWrapper() {
		delete mEvent;
	}

	std::string EventWrapper::GetName()
	{
		return mEvent->GetName();
	}
}