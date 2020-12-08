#pragma once

#define HEALTH_TRACKER_COMPONENT "health_tracker"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"

#include "engine/systems/events/EventHandler.h"

#include <rapidjson/document.h>

// Forward declarations
namespace VannoEngine {
	class GraphicsManager;
	class Event;
}

class HealthTracker : public VannoEngine::GameComponent, public VannoEngine::EventHandler
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

	bool IsInvulnerable() { return mInvulnerable; }
	bool IsAlive() { return mCurrentHealth > 0.0f; }

	void HandleLocalEvent(std::string eventName, VannoEngine::Event* event);
	void HandleEvent(std::string eventName, VannoEngine::Event* event);

private:
	VannoEngine::GraphicsManager* mpGraphicsManager;

	float mMaxHealth;
	float mCurrentHealth;

	bool mVisible;

	bool mInvulnerable;

	double mDamageCooldown;
};