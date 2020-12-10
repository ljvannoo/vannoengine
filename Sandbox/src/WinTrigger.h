#pragma once

#define WINTRIGGER_COMPONENT "win_trigger"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"
#include "engine/systems/events/EventHandler.h"

#include <rapidjson/document.h>

class WinTrigger : public VannoEngine::GameComponent, public VannoEngine::EventHandler
{
public:
	WinTrigger(VannoEngine::GameObject* owner);
	~WinTrigger() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return WINTRIGGER_COMPONENT;
	}

	void HandleEvent(std::string eventName, VannoEngine::Event* event) override;

private:
	bool mUnlocked;
};