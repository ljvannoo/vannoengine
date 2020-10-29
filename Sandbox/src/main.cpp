#include <VannoEngine.h>
#include "engine/systems/LevelManager.h"
#include "engine/core/components/Camera.h"

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
	}
};

VannoEngine::Game* VannoEngine::CreateGame() {
	return new Sandbox();
}