#pragma once

#define MAINMENU_COMPONENT "main_menu_controller"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"

#include <rapidjson/document.h>

namespace VannoEngine {
	class InputManager;
}
class MainMenuController : public VannoEngine::GameComponent
{
public:
	enum class State : int {
		PlayGame,
		ViewControls,
		QuitGame
	};

	MainMenuController(VannoEngine::GameObject* owner);
	~MainMenuController() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return MAINMENU_COMPONENT;
	}

private:
	VannoEngine::InputManager* mpInputManager;
	State mCurrentState;

	std::string mFirstLevelFilename;
};