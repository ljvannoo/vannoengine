#include <VannoEngine.h>
#include "engine/systems/levels/LevelManager.h"
#include "engine/systems/levels/Level.h"

#include "engine/systems/objects/GameObjectFactory.h"
#include "engine/systems/objects/GameObject.h"

#include "engine/systems/InputManager.h"
#include "engine/components/ComponentCreator.h"
#include "engine/systems/ConfigurationManager.h"

#include "engine/components/Camera.h"

#include "engine/systems/events/EventManager.h"
#include "engine/systems/events/EventHandler.h"

#include "Controller.h"
#include "PowerUp.h"
#include "ArrowController.h"
#include "DebugComponent.h"
#include "HealthTracker.h"
#include "SlimeController.h"
#include "LightBanditController.h"
#include "Sensor.h"
#include "MainMenuController.h"
#include "WinTrigger.h"

#include "EndLevelEvent.h"

class Sandbox : public VannoEngine::Game, VannoEngine::EventHandler {
public:
	Sandbox() :
		pPauseScreen{ nullptr }
	{

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
		pObjectFactory->RegisterComponent(HEALTH_TRACKER_COMPONENT, new VannoEngine::ComponentCreator<HealthTracker>());
		pObjectFactory->RegisterComponent(SLIME_CONTROLLER_COMPONENT, new VannoEngine::ComponentCreator<SlimeController>());
		pObjectFactory->RegisterComponent(LIGHT_BANDIT_CONTROLLER_COMPONENT, new VannoEngine::ComponentCreator<LightBanditController>());
		pObjectFactory->RegisterComponent(SENSOR_COMPONENT, new VannoEngine::ComponentCreator<Sensor>());
		pObjectFactory->RegisterComponent(MAINMENU_COMPONENT, new VannoEngine::ComponentCreator<MainMenuController>());
		pObjectFactory->RegisterComponent(WINTRIGGER_COMPONENT, new VannoEngine::ComponentCreator<WinTrigger>());
		

		pObjectFactory->RegisterComponent(DEBUG_COMPONENT, new VannoEngine::ComponentCreator<DebugComponent>());

		pPauseScreen = pObjectFactory->CreateObject("objects\\pause_screen.json", nullptr);

		// Load the level
		VannoEngine::LevelManager* pLevelManager = VannoEngine::LevelManager::GetInstance();
		pLevelManager->LoadLevel(pConfigurationManager->GetString("/initialLevel"));
		pLevelManager->GetCamera()->SetScreenDimensions(windowWidth, windowHeight);

		VannoEngine::EventManager::GetInstance()->Subscribe(EVT_END_LEVEL, this);
	}

	void HandleInput() override {
		VannoEngine::InputManager* pInputManager = VannoEngine::InputManager::GetInstance();

		if (IsPaused) {
			if (pInputManager->IsKeyTriggered("quit")) {
				VannoEngine::LevelManager* pLevelManager = VannoEngine::LevelManager::GetInstance();
				IsPaused = false;
				pLevelManager->UnloadLevel();
				return;
			}
			else if (pInputManager->IsKeyTriggered("pause")) {
				VannoEngine::LevelManager* pLevelManager = VannoEngine::LevelManager::GetInstance();
				pLevelManager->GetCurrentLevel()->RemoveUiObject(pPauseScreen);
				IsPaused = false;
			}
		}
		else if (pInputManager->IsKeyTriggered("pause")) {
			VannoEngine::LevelManager* pLevelManager = VannoEngine::LevelManager::GetInstance();
			pLevelManager->GetCurrentLevel()->AddUiObject(pPauseScreen);
			IsPaused = true;
		}


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

	void HandleEvent(std::string eventName, VannoEngine::Event* message) {
		if (eventName == EVT_END_LEVEL) {
			VannoEngine::LevelManager* pLevelManager = VannoEngine::LevelManager::GetInstance();
			pLevelManager->UnloadLevel();
			if (!pLevelManager->GetCurrentLevel()) {
				IsRunning = false;
			}
		}
	}
private:
	VannoEngine::GameObject* pPauseScreen;
};

VannoEngine::Game* VannoEngine::CreateGame() {
	return new Sandbox();
}