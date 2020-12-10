#include "PowerupUi.h"

#include "engine/core/Log.h"

#include "engine/systems/events/EventManager.h"
#include "PickupPowerupEvent.h"

#include "engine/components/Animator.h"

PowerupUi::PowerupUi(VannoEngine::GameObject* owner) :
	GameComponent(owner),
	mType{ std::string() }
{ 
	VannoEngine::EventManager::GetInstance()->Subscribe(EVT_PICKUP_POWERUP, this);
}

PowerupUi::~PowerupUi() {
	VannoEngine::EventManager::GetInstance()->Unsubscribe(EVT_PICKUP_POWERUP, this);
}


void PowerupUi::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
	if (pData->HasMember("powerupType") && (*pData)["powerupType"].IsString()) {
		mType = (*pData)["powerupType"].GetString();
	}
}

void PowerupUi::Update(double deltaTime) {

}

void PowerupUi::Draw() {

}

void PowerupUi::HandleEvent(std::string eventName, VannoEngine::Event* event) {
	PowerupPickupEvent* pEvent = dynamic_cast<PowerupPickupEvent*>(event);
	if (pEvent->GetPowerup()->GetName() == mType) {
		VannoEngine::Animator* pAnimator = dynamic_cast<VannoEngine::Animator*>(GetOwner()->GetComponent(ANIMATOR_COMPONENT));
		if (pAnimator) {
			pAnimator->Play("selected");
		}
	}
}
