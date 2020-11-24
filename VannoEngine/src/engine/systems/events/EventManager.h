#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		EventManager.h
Purpose:		Responsible for managing game events
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-13
*************************************************************************/

#include "engine/systems/events/EventWrapper.h"

#include <string>
#include <queue>
#include <vector>
#include <unordered_map>


namespace VannoEngine {
	class EventHandler;

	class EventComparator {
	public:
		bool operator()(const EventWrapper& lhs, EventWrapper& rhs) const {
			return (lhs.GetTime() > rhs.GetTime());
		}
	};

	class EventManager
	{
	public:
	public:
		static EventManager* GetInstance();
		~EventManager();

		void Update();
		void Subscribe(std::string eventName, EventHandler* handler);
		void Unsubscribe(std::string eventName, EventHandler* handler);
		void Broadcast(Event* message);
		void DelayedBroadcast(double delaySec, Event* message);
	private:
		EventManager() {}
	private:
		static EventManager* mpInstance;
		std::priority_queue<EventWrapper, std::vector<EventWrapper>, EventComparator > mEvents;
		std::unordered_map<std::string, std::vector<EventHandler*>*> mSubscribers;
	};
}