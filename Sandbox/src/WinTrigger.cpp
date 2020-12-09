#include "WinTrigger.h"

#include "engine/core/Log.h"

#include "engine/systems/physics/ObjectCollisionEvent.h"

#include "engine/systems/events/EventManager.h"
#include "WinEvent.h"

#include "engine/components/PhysicsBody.h"

WinTrigger::WinTrigger(VannoEngine::GameObject* owner) :
	GameComponent(owner)
{
	VannoEngine::EventManager::GetInstance()->Subscribe(EVT_OBJECT_COLLISION, this);
}

WinTrigger::~WinTrigger() {
	VannoEngine::EventManager::GetInstance()->Unsubscribe(EVT_OBJECT_COLLISION, this);
}


void WinTrigger::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
}

void WinTrigger::Update(double deltaTime) {
}

void WinTrigger::Draw() {
}

void WinTrigger::HandleEvent(std::string eventName, VannoEngine::Event* event) {
	if (event->GetName() == EVT_OBJECT_COLLISION) {
		VannoEngine::ObjectCollisionEvent* pCollisionEvent = dynamic_cast<VannoEngine::ObjectCollisionEvent*>(event);
		VannoEngine::PhysicsBody* pBody = dynamic_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));
		VannoEngine::PhysicsBody* pOtherBody = pCollisionEvent->GetOtherBody();

		if (pCollisionEvent->GetBody() == pBody && pOtherBody->GetPhysicsLayer() == "player") {
			VannoEngine::EventManager::GetInstance()->Broadcast(new WinEvent());
		}
	}
}