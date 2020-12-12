#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		EventWrapper.h
Purpose:		Wraps events and adds a time when the event should be triggered
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-23
*************************************************************************/

#include "engine/systems/events/Event.h"
#include <string>
namespace VannoEngine {
	class GameObject;

	class EventWrapper final
	{
	public: // Methods
		EventWrapper(double time, GameObject* pObj, Event* message) :
			mTime{ time },
			mEvent{ message },
			mpObj{ pObj }
		{ }

		~EventWrapper() {
			delete mEvent;
		}

		std::string GetName() { return mEvent->GetName(); }
		double GetTime() const { return mTime; }
		Event* GetEvent() { return mEvent; }
		GameObject* GetObj() { return mpObj; }

	private: // Methods

	private: // Variables
		double mTime;
		GameObject* mpObj;
		Event* mEvent;
	};
}
