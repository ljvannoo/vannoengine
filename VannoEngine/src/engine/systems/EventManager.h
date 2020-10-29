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

#include <string>
#include <queue>
#include <vector>
#include <unordered_map>



#define EVT_MOVE "move"

namespace VannoEngine {
	class EventHandler;

	struct Event {
		double time;
		std::string name;
		std::string data;
	};

	class EventComparator {
	public:
		bool operator()(const Event& lhs, Event& rhs) const {
			return (lhs.time > rhs.time);
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
		void Notify(std::string eventName, std::string data);
		void DelayedNotify(double delaySec, std::string eventName, std::string data);
	private:
		EventManager() {}
	private:
		static EventManager* mpInstance;
		std::priority_queue<Event, std::vector<Event>, EventComparator > mEvents;
		std::unordered_map<std::string, std::vector<EventHandler*>*> mSubscribers;
	};
}