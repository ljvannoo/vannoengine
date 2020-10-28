#pragma once

#include "engine/core.h"
#include "engine/systems/EventHandler.h"

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


	class ENGINE_API Game : EventHandler
	{
	public: 
		bool IsRunning;

	public:
		Game();
		virtual ~Game();

		virtual void Init(int fpsCap);
		void Run();
	private:
		void Update();
		void Draw();

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

