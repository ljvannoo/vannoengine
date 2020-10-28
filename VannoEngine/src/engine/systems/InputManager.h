#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		InputManager.h
Purpose:		Provides functionality for collecting user input.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-05
*************************************************************************/
#include "engine/core.h"
#include <SDL_keyboard.h>

namespace VannoEngine {
	class EventManager;

	class ENGINE_API InputManager
	{
	public:
		static InputManager* GetInstance();
		~InputManager();

		void Init();
		void Update();

		void HandleInput();

		bool IsKeyTriggered(unsigned int scancode);
		bool IsKeyPressed(unsigned int scancode);
		bool IsKeyReleased(unsigned int scancode);
	private:
		InputManager();

	private:
		static InputManager* mpInstance;

		Uint8 mCurrentState[512];
		Uint8 mPreviousState[512];

		EventManager* mpEventManager;
	};
}