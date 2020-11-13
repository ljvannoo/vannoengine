#include <VannoEngine.h>
#include "engine/systems/levels/LevelManager.h"
#include "engine/systems/objects/GameObjectFactory.h"
#include "engine/systems/InputManager.h"
#include "engine/components/ComponentCreator.h"
#include "engine/systems/ConfigurationManager.h"

#include "engine/components/Camera.h"

#include "Controller.h"

class Sandbox : public VannoEngine::Game {
public:
	Sandbox() {

	}

	~Sandbox() {

	}

	void Init() {
		VannoEngine::ConfigurationManager* pConfigurationManager = VannoEngine::ConfigurationManager::GetInstance();
		pConfigurationManager->Init("config.json");

		int windowWidth = pConfigurationManager->GetInt("/window/width");
		int windowHeight = pConfigurationManager->GetInt("/window/height");

		VannoEngine::Game::Init(windowWidth, windowHeight, pConfigurationManager->GetInt("/maxFrameRate"));
		
		// Add custom components
		VannoEngine::GameObjectFactory* pObjectFactory = VannoEngine::GameObjectFactory::GetInstance();
		pObjectFactory->RegisterComponent(CONTROLLER_COMPONENT, new VannoEngine::ComponentCreator<Controller>());

		// Load the level
		VannoEngine::LevelManager* pLevelManager = VannoEngine::LevelManager::GetInstance();
		pLevelManager->LoadLevel(pConfigurationManager->GetString("/initialLevel"));
		pLevelManager->GetCamera()->SetScreenDimensions(windowWidth, windowHeight);
	}
};

VannoEngine::Game* VannoEngine::CreateGame() {
	return new Sandbox();
}