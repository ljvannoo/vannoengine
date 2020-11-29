#pragma once

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

