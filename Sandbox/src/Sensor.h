#pragma once

#define SENSOR_COMPONENT "sensor"

#include "engine/components/GameComponent.h"
#include "engine/systems/objects/GameObject.h"

#include "engine/systems/events/EventHandler.h"

#include <rapidjson/document.h>

// Forward declarations
namespace VannoEngine {
	class GraphicsManager;
	class Event;
}

class Sensor : public VannoEngine::GameComponent, public VannoEngine::EventHandler
{
public:
	Sensor(VannoEngine::GameObject* owner);
	~Sensor() override;

	void Update(double deltaTime);
	virtual void Draw();

	void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

	std::string GetType() override {
		return SENSOR_COMPONENT;
	}

	void HandleEvent(std::string eventName, VannoEngine::Event* event);
	void HandleLocalEvent(std::string eventName, VannoEngine::Event* event);

private:

};