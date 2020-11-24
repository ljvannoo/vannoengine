#pragma once
#define POWERUP_COMPONENT "powerup"

#include "engine/components/GameComponent.h"

// Forward declarations
namespace VannoEngine {
	class GameObject;
}

class PowerUp : public VannoEngine::GameComponent
{
public: // Variables

public: // Methods
	PowerUp(VannoEngine::GameObject* owner);
	~PowerUp();

	virtual std::string GetType() override { return POWERUP_COMPONENT; }

	virtual void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

private: // Methods

private: // Variables
};
