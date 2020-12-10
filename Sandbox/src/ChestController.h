#pragma once

#define CHEST_CONTROLLER_COMPONENT "chest_controller"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"

#include <rapidjson/document.h>

class ChestController : public VannoEngine::GameComponent
{
public:
	enum class State : int {
		Closed,
		Opening,
		Open
	};

	ChestController(VannoEngine::GameObject* owner);
	~ChestController() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return CHEST_CONTROLLER_COMPONENT;
	}

	void HandleLocalEvent(std::string eventName, VannoEngine::Event* event) override;

private:
	std::string mContents;

	State mCurrentState;
};