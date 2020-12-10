#pragma once

#define POWERUP_UI_COMPONENT "powerup_ui"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"
#include "engine/systems/events/EventHandler.h"

#include <rapidjson/document.h>

class PowerupUi : public VannoEngine::GameComponent, public VannoEngine::EventHandler
{
public:
	PowerupUi(VannoEngine::GameObject* owner);
	~PowerupUi() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return POWERUP_UI_COMPONENT;
	}

	void HandleEvent(std::string eventName, VannoEngine::Event* event) override;
private:
	std::string mType;
};