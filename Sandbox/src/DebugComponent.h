#pragma once

#define DEBUG_COMPONENT "debug"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"

#include <rapidjson/document.h>

class DebugComponent : public VannoEngine::GameComponent
{
public:
	DebugComponent(VannoEngine::GameObject* owner);
	~DebugComponent() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return DEBUG_COMPONENT;
	}

private:
};