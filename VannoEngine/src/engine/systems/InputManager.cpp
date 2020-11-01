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

#include "engine/systems/events/EventManager.h"

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
		// TODO: Generalize this
		if (IsKeyPressed(SDL_SCANCODE_W)) {
			mpEventManager->Notify(EVT_MOVE, "up");
		}

		if (IsKeyPressed(SDL_SCANCODE_A)) {
			mpEventManager->Notify(EVT_MOVE, "left");
		}

		if (IsKeyPressed(SDL_SCANCODE_S)) {
			mpEventManager->Notify(EVT_MOVE, "down");
		}

		if (IsKeyPressed(SDL_SCANCODE_D)) {
			mpEventManager->Notify(EVT_MOVE, "right");
		}

	}

	bool InputManager::IsKeyTriggered(unsigned int scancode) {
		if (scancode < 512 && mCurrentState[scancode] && !mPreviousState[scancode]) {
			return true;
		}
		return false;
	}

	bool InputManager::IsKeyPressed(unsigned int scancode) {
		if (scancode < 512 && mCurrentState[scancode]) {
			return true;
		}
		return false;
	}

	bool InputManager::IsKeyReleased(unsigned int scancode) {
		if (scancode < 512 && !mCurrentState[scancode] && mPreviousState[scancode]) {
			return true;
		}
		return false;
	}
}