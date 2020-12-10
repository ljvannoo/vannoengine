#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Game.h
Purpose:		The main engine class that games should derive from.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-20
*************************************************************************/

#include <string>

namespace VannoEngine {
	class InputManager;
	class FramerateController;
	class EventManager;
	class GraphicsManager;
	class GameObjectFactory;
	class LevelManager;
	class TimeManager;
	class TestSystem;


	class Game
	{

	public:
		Game();
		virtual ~Game();

		void Init(int windowWidth, int windowHeight, int fpsCap);

		// The following methods are available for extension
		virtual void Init() = 0;
		virtual void HandleInput() {}
		virtual void UpdatePhysics(double deltaTime) {}
		virtual void Update(double deltaTime) {}
		virtual void Draw() {}

		void Run();
	protected:
		bool IsRunning;
		bool IsPaused;

	private:
		InputManager* mpInputManager;
		FramerateController* mpFramerateManager;
		EventManager* mpEventManager;
		GraphicsManager* mpGraphicsManager;
		GameObjectFactory* mpObjectFactory;
		LevelManager* mpLevelManager;
		TimeManager* mpTimeManager;
		TestSystem* mpTestSystem;

		bool mIsInitialized;
	};

	Game* CreateGame();
}

