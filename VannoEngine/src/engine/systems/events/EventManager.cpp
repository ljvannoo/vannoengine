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

#include "engine/systems/events/EventWrapper.h"
#include "engine/systems/events/Event.h"

#include "engine/systems/objects/GameObject.h"

#include "engine/core/Log.h"

#include "engine/systems/events/EventHandler.h"

#include "engine/systems/TimeManager.h"


namespace VannoEngine {
	EventManager* EventManager::mpInstance = nullptr;

	EventManager::~EventManager() {
		for (std::pair<std::string, std::list<EventHandler*>*> element : mSubscribers) {
			delete element.second;
		}
		mSubscribers.clear();

		if (mpInstance) {
			delete mpInstance;
		}
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
			EventWrapper* evt = mEvents.top();

			if (timeManager->Now() < evt->GetTime()) {
				break;
			}
			mEvents.pop();
			std::list<EventHandler*>* handlers = mSubscribers[evt->GetName()];
			if (handlers) {
				for (EventHandler* handler : *handlers) {
					handler->HandleEvent(evt->GetName(), evt->GetEvent());
				}
			}
			delete evt;
		}
	}

	void EventManager::Subscribe(std::string eventName, EventHandler* handler) {
		std::list<EventHandler*>* handlers = mSubscribers[eventName];
		if (!handlers) {
			handlers = new std::list<EventHandler*>;
			mSubscribers[eventName] = handlers;
		}

		handlers->push_back(handler);
	}

	void EventManager::Unsubscribe(std::string eventName, EventHandler* handler) {
		std::list<EventHandler*>* handlers = mSubscribers[eventName];
		handlers->remove(handler);
	}

	void EventManager::Broadcast(Event* message) {
		DelayedBroadcast(0.0, message);
	}

	void EventManager::DelayedBroadcast(double delaySec, Event* message) {
		TimeManager* timeManager = TimeManager::GetInstance();
		// TODO: Event execution time is set up front. This is incompatible with a pause feature.
		EventWrapper* evt = new EventWrapper(timeManager->Now() + delaySec, message);
		mEvents.push(evt);
	}

	void EventManager::Direct(GameObject* pObj, Event* event) {
		pObj->HandleLocalEvent(event->GetName(), event);
		delete event;
	}
}