#pragma once
#pragma once

#define ARROW_CONTROLLER_COMPONENT "arrow_controller"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"
#include "engine/systems/events/EventHandler.h"

#include <rapidjson/document.h>

class ArrowController : public VannoEngine::GameComponent, public VannoEngine::EventHandler
{
public:
	ArrowController(VannoEngine::GameObject* owner);
	~ArrowController() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	void HandleEvent(std::string eventName, VannoEngine::Event* event);

	std::string GetType() override {
		return ARROW_CONTROLLER_COMPONENT;
	}

private:
};