#include "DebugComponent.h"

#include "engine/core/Log.h"

DebugComponent::DebugComponent(VannoEngine::GameObject* owner) :
	GameComponent(owner)
{ }

DebugComponent::~DebugComponent()
{ }


void DebugComponent::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
}

void DebugComponent::Update(double deltaTime) {
	LOG_DEBUG("Here!");
}

void DebugComponent::Draw() {

}

