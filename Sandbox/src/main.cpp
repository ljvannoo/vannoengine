#include <VannoEngine.h>
#include "engine/systems/levels/LevelManager.h"
#include "engine/systems/objects/GameObjectFactory.h"
#include "engine/systems/InputManager.h"
#include "engine/components/ComponentCreator.h"
#include "engine/systems/ConfigurationManager.h"

#include "engine/components/Camera.h"

#include "Controller.h"
#include "PowerUp.h"
#include "ArrowController.h"
#include "DebugComponent.h"

class Sandbox : public VannoEngine::Game {
public:
	Sandbox() {

	}

	~Sandbox() {

	}

	void Init() override {
		VannoEngine::ConfigurationManager* pConfigurationManager = VannoEngine::ConfigurationManager::GetInstance();
		pConfigurationManager->Init("config.json");

		float windowWidth = pConfigurationManager->GetFloat("/window/width");
		float windowHeight = pConfigurationManager->GetFloat("/window/height");

		VannoEngine::Game::Init((int)windowWidth, (int)windowHeight, pConfigurationManager->GetInt("/maxFrameRate"));
		
		// Add custom components
		VannoEngine::GameObjectFactory* pObjectFactory = VannoEngine::GameObjectFactory::GetInstance();
		pObjectFactory->RegisterComponent(CONTROLLER_COMPONENT, new VannoEngine::ComponentCreator<Controller>());
		pObjectFactory->RegisterComponent(POWERUP_COMPONENT, new VannoEngine::ComponentCreator<PowerUp>());
		pObjectFactory->RegisterComponent(ARROW_CONTROLLER_COMPONENT, new VannoEngine::ComponentCreator<ArrowController>());
		pObjectFactory->RegisterComponent(DEBUG_COMPONENT, new VannoEngine::ComponentCreator<DebugComponent>());

		// Load the level
		VannoEngine::LevelManager* pLevelManager = VannoEngine::LevelManager::GetInstance();
		pLevelManager->LoadLevel(pConfigurationManager->GetString("/initialLevel"));
		pLevelManager->GetCamera()->SetScreenDimensions(windowWidth, windowHeight);
	}

	void HandleInput() override {
		VannoEngine::InputManager* pInputManager = VannoEngine::InputManager::GetInstance();

		if (pInputManager->IsKeyTriggered("quit")) {
			IsRunning = false;
		}

		// TODO: Fix debug mode
		if (pInputManager->IsKeyTriggered("debug")) {
			VannoEngine::ConfigurationManager* pConfigrationManager = VannoEngine::ConfigurationManager::GetInstance();
			pConfigrationManager->ToggleBool("/debugMode");
			LOG_DEBUG("Debug Mode is now: {0}", pConfigrationManager->GetBool("/debugMode"));
		}

		if (pInputManager->IsKeyTriggered("zoomIn")) {
			VannoEngine::LevelManager* pLevelManager = VannoEngine::LevelManager::GetInstance();
			float scale = pLevelManager->GetCamera()->GetScale();
			pLevelManager->GetCamera()->SetScale(scale + 0.25f);
		} else if (pInputManager->IsKeyTriggered("zoomOut")) {
			VannoEngine::LevelManager* pLevelManager = VannoEngine::LevelManager::GetInstance();
			float scale = pLevelManager->GetCamera()->GetScale();
			scale -= 0.25f;
			if (scale < 1.0f) {
				scale = 1.0f;
			}
			pLevelManager->GetCamera()->SetScale(scale);

		}
	}
};

VannoEngine::Game* VannoEngine::CreateGame() {
	return new Sandbox();
}