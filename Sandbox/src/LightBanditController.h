#pragma once

#define LIGHT_BANDIT_CONTROLLER_COMPONENT "light_bandit_controller"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"
#include "engine/systems/events/EventHandler.h"

#include <rapidjson/document.h>

class LightBanditController : public VannoEngine::GameComponent, public VannoEngine::EventHandler
{
private:
	enum class State : int {
		Idle,
		Alert,
		Dieing,
		Dead
	};
public:
	LightBanditController(VannoEngine::GameObject* owner);
	~LightBanditController() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	void HandleEvent(std::string eventName, VannoEngine::Event* event);
	void HandleLocalEvent(std::string eventName, VannoEngine::Event* event);

	std::string GetType() override {
		return LIGHT_BANDIT_CONTROLLER_COMPONENT;
	}

	void TurnAround();
private:
	float MoveTowards(float current, float target, float maxDelta);
private:
	const float cWalkSpeed = 1.0f;
	const float cWalkAccel = 60.0f;
	const double cDamageCooldown = 1.0;

	float mDirection = 1.0f;

	double mCooldown;

	const double cAlertCooldown = 5.0;
	double mAlertCooldown;

	State mCurrentState;
};