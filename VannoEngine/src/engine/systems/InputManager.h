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

#include <unordered_map>

namespace VannoEngine {
	class EventManager;
	
	enum class Key {W, A, S, D};
	class InputManager
	{
	public:
		static InputManager* GetInstance();
		~InputManager();

		void Init();
		
		void RegisterKey(Key key, std::string action);
		
		void Update();

		void HandleInput();

		bool IsKeyTriggered(unsigned int scancode);
		bool IsKeyPressed(unsigned int scancode);
		bool IsKeyReleased(unsigned int scancode);
	private:
		InputManager();

	private:
		static InputManager* mpInstance;

		unsigned int mCurrentState[512];
		unsigned int mPreviousState[512];

		EventManager* mpEventManager;

		std::unordered_map<unsigned int, std::string> mKeyRegistry;
		std::unordered_map<Key, unsigned int> mScanCodes;
	};
}