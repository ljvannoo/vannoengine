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
#include "engine/systems/ConfigurationManager.h"

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
		mpConfigurationManager = ConfigurationManager::GetInstance();

		memset(mCurrentState, 0, 512 * sizeof(Uint8));
		memset(mPreviousState, 0, 512 * sizeof(Uint8));

		mScanCodes["0"] = SDL_SCANCODE_0;
		mScanCodes["1"] = SDL_SCANCODE_1;
		mScanCodes["2"] = SDL_SCANCODE_2;
		mScanCodes["3"] = SDL_SCANCODE_3;
		mScanCodes["4"] = SDL_SCANCODE_4;
		mScanCodes["5"] = SDL_SCANCODE_5;
		mScanCodes["6"] = SDL_SCANCODE_6;
		mScanCodes["7"] = SDL_SCANCODE_7;
		mScanCodes["8"] = SDL_SCANCODE_8;
		mScanCodes["9"] = SDL_SCANCODE_9;
		mScanCodes["a"] = SDL_SCANCODE_A;
		mScanCodes["b"] = SDL_SCANCODE_B;
		mScanCodes["c"] = SDL_SCANCODE_C;
		mScanCodes["d"] = SDL_SCANCODE_D;
		mScanCodes["e"] = SDL_SCANCODE_E;
		mScanCodes["f"] = SDL_SCANCODE_F;
		mScanCodes["g"] = SDL_SCANCODE_G;
		mScanCodes["h"] = SDL_SCANCODE_H;
		mScanCodes["i"] = SDL_SCANCODE_I;
		mScanCodes["j"] = SDL_SCANCODE_J;
		mScanCodes["k"] = SDL_SCANCODE_K;
		mScanCodes["l"] = SDL_SCANCODE_L;
		mScanCodes["m"] = SDL_SCANCODE_M;
		mScanCodes["n"] = SDL_SCANCODE_N;
		mScanCodes["o"] = SDL_SCANCODE_O;
		mScanCodes["p"] = SDL_SCANCODE_P;
		mScanCodes["q"] = SDL_SCANCODE_Q;
		mScanCodes["r"] = SDL_SCANCODE_R;
		mScanCodes["s"] = SDL_SCANCODE_S;
		mScanCodes["t"] = SDL_SCANCODE_T;
		mScanCodes["u"] = SDL_SCANCODE_U;
		mScanCodes["v"] = SDL_SCANCODE_V;
		mScanCodes["w"] = SDL_SCANCODE_W;
		mScanCodes["x"] = SDL_SCANCODE_X;
		mScanCodes["y"] = SDL_SCANCODE_Y;
		mScanCodes["z"] = SDL_SCANCODE_Z;
		mScanCodes["."] = SDL_SCANCODE_PERIOD;
		mScanCodes[","] = SDL_SCANCODE_COMMA;
		mScanCodes["."] = SDL_SCANCODE_PERIOD;
		mScanCodes["'"] = SDL_SCANCODE_APOSTROPHE;
		mScanCodes["f1"] = SDL_SCANCODE_F1;
		mScanCodes["f2"] = SDL_SCANCODE_F2;
		mScanCodes["f3"] = SDL_SCANCODE_F3;
		mScanCodes["f4"] = SDL_SCANCODE_F4;
		mScanCodes["f5"] = SDL_SCANCODE_F5;
		mScanCodes["f6"] = SDL_SCANCODE_F6;
		mScanCodes["f7"] = SDL_SCANCODE_F7;
		mScanCodes["f8"] = SDL_SCANCODE_F8;
		mScanCodes["f9"] = SDL_SCANCODE_F9;
		mScanCodes["f10"] = SDL_SCANCODE_F10;
		mScanCodes["f11"] = SDL_SCANCODE_F11;
		mScanCodes["f12"] = SDL_SCANCODE_F12;
		mScanCodes["space"] = SDL_SCANCODE_SPACE;
		mScanCodes["esc"] = SDL_SCANCODE_ESCAPE;
		mScanCodes["enter"] = SDL_SCANCODE_RETURN;
		mScanCodes["tab"] = SDL_SCANCODE_TAB;
		mScanCodes["lshift"] = SDL_SCANCODE_LSHIFT;
		mScanCodes["lctrl"] = SDL_SCANCODE_LCTRL;
		mScanCodes["lalt"] = SDL_SCANCODE_LALT;
		mScanCodes["up_arrow"] = SDL_SCANCODE_UP;
		mScanCodes["down_arrow"] = SDL_SCANCODE_DOWN;
		mScanCodes["left_arrow"] = SDL_SCANCODE_LEFT;
		mScanCodes["right_arrow"] = SDL_SCANCODE_RIGHT;
		
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
	}

	bool InputManager::IsKeyTriggered(std::string action) {
		std::string key = mpConfigurationManager->GetString("/keyMapping/" + action);

		if (key != "") {
			unsigned int scancode = mScanCodes[key];

			if (scancode < 512 && mCurrentState[scancode] && !mPreviousState[scancode]) {
				return true;
			}
		}
		return false;
	}

	bool InputManager::IsKeyPressed(std::string action) {
		std::string key = mpConfigurationManager->GetString("/keyMapping/" + action);

		if (key != "") {
			unsigned int scancode = mScanCodes[key];
			if (scancode < 512 && mCurrentState[scancode]) {
				return true;
			}
		}
		return false;
	}

	bool InputManager::IsKeyReleased(std::string action) {
		std::string key = mpConfigurationManager->GetString("/keyMapping/" + action);

		if (key != "") {
			unsigned int scancode = mScanCodes[key];
			if (scancode < 512 && !mCurrentState[scancode] && mPreviousState[scancode]) {
				return true;
			}
		}
		return false;
	}

	bool InputManager::IsAnyKeyTriggered() {
		for (int i = 0; i < 512; ++i) {
			if (mCurrentState[i] && !mPreviousState[i]) {
				return true;
			}
		}
		return false;
	}
}