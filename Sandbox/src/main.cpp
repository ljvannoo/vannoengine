#include <VannoEngine.h>
#include "engine/systems/levels/LevelManager.h"
#include "engine/systems/InputManager.h"
#include "engine/components/Camera.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS_CAP 60

class Sandbox : public VannoEngine::Game {
public:
	Sandbox() {

	}

	~Sandbox() {

	}

	void Init() {
		VannoEngine::Game::Init(WINDOW_WIDTH, WINDOW_HEIGHT, FPS_CAP);

		VannoEngine::LevelManager* pLevelManager = VannoEngine::LevelManager::GetInstance();

		pLevelManager->LoadLevel("levels\\demo_level1.json");
		pLevelManager->GetCamera()->SetScreenDimensions(WINDOW_WIDTH, WINDOW_HEIGHT);

		VannoEngine::InputManager* pInputManager = VannoEngine::InputManager::GetInstance();
		pInputManager->RegisterKey(VannoEngine::Key::W, "move_up");
		pInputManager->RegisterKey(VannoEngine::Key::A, "move_left");
		pInputManager->RegisterKey(VannoEngine::Key::S, "move_down");
		pInputManager->RegisterKey(VannoEngine::Key::D, "move_right");
	}
};

VannoEngine::Game* VannoEngine::CreateGame() {
	return new Sandbox();
}