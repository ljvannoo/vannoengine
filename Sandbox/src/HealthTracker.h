#pragma once

#define HEALTH_TRACKER_COMPONENT "health_tracker"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"

#include <rapidjson/document.h>

// Forward declarations
namespace VannoEngine {
	class GraphicsManager;
	class Event;
}

class HealthTracker : public VannoEngine::GameComponent
{
public:
	HealthTracker(VannoEngine::GameObject* owner);
	~HealthTracker() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return HEALTH_TRACKER_COMPONENT;
	}

	void HandleLocalEvent(std::string eventName, VannoEngine::Event* event);

private:
	VannoEngine::GraphicsManager* mpGraphicsManager;

	float mMaxHealth;
	float mCurrentHealth;
};