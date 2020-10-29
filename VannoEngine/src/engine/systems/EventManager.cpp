/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		EventManager.cpp
Purpose:		Responsible for managing game events
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-13
*************************************************************************/
#include "EventManager.h"
#include "TimeManager.h"
#include "EventHandler.h"



namespace VannoEngine {
	EventManager* EventManager::mpInstance = nullptr;

	EventManager::~EventManager() {
		for (std::pair<std::string, std::vector<EventHandler*>*> element : mSubscribers) {
			delete element.second;
		}
		mSubscribers.clear();
	}

	EventManager* EventManager::GetInstance() {
		if (!mpInstance) {
			mpInstance = new EventManager;
		}

		return mpInstance;
	}

	void EventManager::Update() {
		TimeManager* timeManager = TimeManager::GetInstance();
		while (!mEvents.empty()) {
			Event evt = mEvents.top();

			if (timeManager->Now() < evt.time) {
				break;
			}
			mEvents.pop();
			std::vector<EventHandler*>* handlers = mSubscribers[evt.name];
			if (handlers) {
				for (EventHandler* handler : *handlers) {
					handler->HandleEvent(evt.name, evt.data);
				}
			}
		}
	}

	void EventManager::Subscribe(std::string eventName, EventHandler* handler) {
		std::vector<EventHandler*>* handlers = mSubscribers[eventName];
		if (!handlers) {
			handlers = new std::vector<EventHandler*>;
			mSubscribers[eventName] = handlers;
		}

		handlers->push_back(handler);
	}

	void EventManager::Unsubscribe(std::string eventName, EventHandler* handler) {
	}

	void EventManager::Notify(std::string eventName, std::string data) {
		DelayedNotify(0.0, eventName, data);
	}

	void EventManager::DelayedNotify(double delaySec, std::string eventName, std::string data) {
		TimeManager* timeManager = TimeManager::GetInstance();
		Event evt = { timeManager->Now() + delaySec, eventName, data };
		mEvents.push(evt);
	}
}