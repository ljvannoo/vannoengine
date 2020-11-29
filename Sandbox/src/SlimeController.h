#pragma once

#define SLIME_CONTROLLER_COMPONENT "slime_controller"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"
#include "engine/systems/events/EventHandler.h"

#include <rapidjson/document.h>

class SlimeController : public VannoEngine::GameComponent, public VannoEngine::EventHandler
{
private:
	enum class State : int {
		Idle,
		Walk,
		Dieing,
		Dead
	};
public:
	SlimeController(VannoEngine::GameObject* owner);
	~SlimeController() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	void HandleEvent(std::string eventName, VannoEngine::Event* event);
	void HandleLocalEvent(std::string eventName, VannoEngine::Event* event);

	std::string GetType() override {
		return SLIME_CONTROLLER_COMPONENT;
	}

	void TurnAround();
private:
	float MoveTowards(float current, float target, float maxDelta);
	void SpawnMinis();
private:
	const float cWalkSpeed = 1.0f;
	const float cWalkAccel = 60.0f;
	const double cDamageCooldown = 1.0;

	float mDirection = 1.0f;

	double mCooldown;

	State mCurrentState;
};