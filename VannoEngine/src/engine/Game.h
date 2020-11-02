#pragma once


#include "engine/systems/events/EventHandler.h"

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


	class Game : EventHandler
	{
	public: 
		bool IsRunning;

	public:
		Game();
		virtual ~Game();

		virtual void Init() = 0;
		void Init(int windowWidth, int windowHeight, int fpsCap);
		void Run();
	private:

		void HandleEvent(std::string eventName, std::string data);
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

