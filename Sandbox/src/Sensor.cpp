#include "Sensor.h"

#include "TurnAroundEvent.h"
#include "DetectedEnemyEvent.h"

#include "engine/systems/events/EventManager.h"
#include "engine/systems/physics/ObjectCollisionEvent.h"

#include "engine/components/PhysicsBody.h"

#include "engine/core/Log.h"

#include <glm/vec2.hpp>

Sensor::Sensor(VannoEngine::GameObject* owner) :
	GameComponent(owner)
{
	VannoEngine::EventManager::GetInstance()->Subscribe(EVT_OBJECT_COLLISION, this);
}

Sensor::~Sensor() {
	VannoEngine::EventManager::GetInstance()->Unsubscribe(EVT_OBJECT_COLLISION, this);
}


void Sensor::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
	
}

void Sensor::Update(double deltaTime) {
	
}

void Sensor::Draw() {
	
}

void Sensor::HandleEvent(std::string eventName, VannoEngine::Event* event) {
	if (eventName == EVT_OBJECT_COLLISION) {
		VannoEngine::ObjectCollisionEvent* pEvent = dynamic_cast<VannoEngine::ObjectCollisionEvent*>(event);
		VannoEngine::GameObject* pObj = pEvent->GetBody()->GetOwner();
		VannoEngine::GameObject* pOther = pEvent->GetOtherBody()->GetOwner();

		if(pObj == GetOwner() && pOther != pObj->GetParentObject() && pEvent->GetOtherBody()->GetPhysicsLayer() == "player") {
			VannoEngine::EventManager::GetInstance()->Direct(pObj->GetParentObject(), new DetectedEnemyEvent(pObj->GetParentObject(), pOther));
		}
	}
}

void Sensor::HandleLocalEvent(std::string eventName, VannoEngine::Event* event) {
	if (eventName == EVT_TURN_AROUND) {
		TurnAroundEvent* pEvent = dynamic_cast<TurnAroundEvent*>(event);
		if (GetOwner()->HasComponent(PHYSICSBODY_COMPONENT)) {
			VannoEngine::PhysicsBody* pBody = dynamic_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));

			glm::vec2 offset = pBody->GetAabbOffset();
			offset.x *= -1;
			pBody->SetAabbOffset(offset.x, offset.y);
		}
		
	}
}
