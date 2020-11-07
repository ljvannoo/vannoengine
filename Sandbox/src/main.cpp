#include <VannoEngine.h>
#include "engine/systems/levels/LevelManager.h"
#include "engine/systems/objects/GameObjectFactory.h"
#include "engine/systems/InputManager.h"
#include "engine/components/ComponentCreator.h"

#include "engine/components/Camera.h"

#include "Controller.h"

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
		
		// Add custom components
		VannoEngine::GameObjectFactory* pObjectFactory = VannoEngine::GameObjectFactory::GetInstance();
		pObjectFactory->RegisterComponent(CONTROLLER_COMPONENT, new VannoEngine::ComponentCreator<Controller>());

		// Register keys
		VannoEngine::InputManager* pInputManager = VannoEngine::InputManager::GetInstance();
		pInputManager->RegisterAction(VannoEngine::Action::MOVE_UP, VannoEngine::Key::W);
		pInputManager->RegisterAction(VannoEngine::Action::MOVE_DOWN, VannoEngine::Key::S);
		pInputManager->RegisterAction(VannoEngine::Action::MOVE_LEFT, VannoEngine::Key::A);
		pInputManager->RegisterAction(VannoEngine::Action::MOVE_RIGHT, VannoEngine::Key::D);
		pInputManager->RegisterAction(VannoEngine::Action::JUMP, VannoEngine::Key::SPACE);

		// Load the level
		VannoEngine::LevelManager* pLevelManager = VannoEngine::LevelManager::GetInstance();
		pLevelManager->LoadLevel("levels\\demo_level1.json");
		pLevelManager->GetCamera()->SetScreenDimensions(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
};

VannoEngine::Game* VannoEngine::CreateGame() {
	return new Sandbox();
}