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
	
	enum class Key {W, A, S, D, SPACE};
	enum class Action {
		MOVE_UP,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		JUMP,
		FIRE
	};
	class InputManager
	{
	public:
		static InputManager* GetInstance();
		~InputManager();

		void Init();
		
		void RegisterAction(Action action, Key key);
		bool IsActionRegistered(Action action);
		
		void Update();

		void HandleInput();

		bool IsKeyTriggered(Action action);
		bool IsKeyPressed(Action action);
		bool IsKeyReleased(Action action);
	private:
		InputManager();

	private:
		static InputManager* mpInstance;

		unsigned char mCurrentState[512];
		unsigned char mPreviousState[512];

		EventManager* mpEventManager;

		std::unordered_map<Action, Key> mActionRegistry;
		std::unordered_map<Key, unsigned char> mScanCodes;
	};
}