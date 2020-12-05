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
	class ConfigurationManager;
	
	class InputManager
	{
	public:
		static InputManager* GetInstance();
		~InputManager();

		void Init();

		void Update();

		void HandleInput();

		bool IsKeyTriggered(std::string action);
		bool IsKeyPressed(std::string action);
		bool IsKeyReleased(std::string action);
		bool IsAnyKeyTriggered();
	private:
		InputManager();

	private:
		static InputManager* mpInstance;

		unsigned char mCurrentState[512];
		unsigned char mPreviousState[512];

		EventManager* mpEventManager;
		ConfigurationManager* mpConfigurationManager;

		std::unordered_map<std::string, unsigned char> mScanCodes;
	};
}