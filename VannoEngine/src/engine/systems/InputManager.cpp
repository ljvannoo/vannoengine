/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		InputManager.cpp
Purpose:		Implementation of the InputManager
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-05
*************************************************************************/

#include "InputManager.h"

#include "engine/core/Log.h"

#include "engine/systems/events/EventManager.h"

#include <SDL_keyboard.h>

#include <memory.h>

namespace VannoEngine {
	InputManager* InputManager::mpInstance = nullptr;

	InputManager* InputManager::GetInstance() {
		if (!mpInstance) {
			mpInstance = new InputManager;
		}

		return mpInstance;
	}

	InputManager::InputManager() {
		mpEventManager = EventManager::GetInstance();
	}

	InputManager::~InputManager() {

	}

	void InputManager::Init() {
		memset(mCurrentState, 0, 512 * sizeof(Uint8));
		memset(mPreviousState, 0, 512 * sizeof(Uint8));

		mScanCodes[Key::W] = SDL_SCANCODE_W;
		mScanCodes[Key::A] = SDL_SCANCODE_A;
		mScanCodes[Key::S] = SDL_SCANCODE_S;
		mScanCodes[Key::D] = SDL_SCANCODE_D;
		mScanCodes[Key::SPACE] = SDL_SCANCODE_SPACE;
	}

	void InputManager::RegisterAction(Action action, Key key) {
		mActionRegistry[action] = key;
	}

	bool InputManager::IsActionRegistered(Action action) {
		return mActionRegistry.find(action) != mActionRegistry.end();
	}

	void InputManager::Update() {
		int count = 0;
		const Uint8* currentKeyStates = SDL_GetKeyboardState(&count);
		
		if (count > 512) {
			count = 512;
		}
		memcpy(mPreviousState, mCurrentState, 512 * sizeof(Uint8));
		memcpy(mCurrentState, currentKeyStates, count * sizeof(Uint8));
	}

	void InputManager::HandleInput() {
		Update();
		/*
		LOG_CORE_DEBUG("There are {0} registered keys", mKeyRegistry.size());
		for (auto pair : mKeyRegistry) {
			if (IsKeyPressed(pair.first)) {
				mpEventManager->Notify(EVT_INPUT, pair.second);
			}
		}
		*/
	}

	bool InputManager::IsKeyTriggered(Action action) {
		if(IsActionRegistered(action)) {
			Key key = mActionRegistry[action];
			unsigned int scancode = mScanCodes[key];
			
			if (scancode < 512 && mCurrentState[scancode] && !mPreviousState[scancode]) {
				return true;
			}
		}
		return false;
	}

	bool InputManager::IsKeyPressed(Action action) {
		if (IsActionRegistered(action)) {
			Key key = mActionRegistry[action];
			unsigned int scancode = mScanCodes[key];
			if (scancode < 512 && mCurrentState[scancode]) {
				return true;
			}
		}
		return false;
	}

	bool InputManager::IsKeyReleased(Action action) {
		if (IsActionRegistered(action)) {
			Key key = mActionRegistry[action];
			unsigned int scancode = mScanCodes[key];
			if (scancode < 512 && !mCurrentState[scancode] && mPreviousState[scancode]) {
				return true;
			}
		}
		return false;
	}
}