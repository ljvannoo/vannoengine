#include "ArrowController.h"

#include "engine/systems/events/EventManager.h"

#include "engine/components/PhysicsBody.h"
#include "engine/components/Transform.h"
#include "engine/systems/physics/MapCollisionEvent.h"

#include "engine/core/Log.h"

#include <glm/gtx/vector_angle.hpp>

ArrowController::ArrowController(VannoEngine::GameObject* owner) :
	GameComponent(owner)
{ 
	VannoEngine::EventManager::GetInstance()->Subscribe(EVT_MAP_COLLISION, this);
}

ArrowController::~ArrowController() { 
	VannoEngine::EventManager::GetInstance()->Unsubscribe(EVT_MAP_COLLISION, this);
}


void ArrowController::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
}

void ArrowController::Update(double deltaTime) {
	VannoEngine::Transform* pTransform = dynamic_cast<VannoEngine::Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));
	glm::vec2 forward(1.0f, 0.0f);
	glm::vec2 speed = pTransform->GetSpeed();
	speed = glm::normalize(speed);
	if(glm::length(speed) > 0.0f) {
		pTransform->SetRotation(glm::degrees(glm::angle(speed, forward)));
	}

}

void ArrowController::Draw() {
	/*if (GetOwner()->HasComponent(PHYSICSBODY_COMPONENT)) {
		VannoEngine::PhysicsBody* pBody = dynamic_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));

		LOG_DEBUG("Pressing against... FLOOR={}, CEILING={}, RIGHT={}, LEFT={}",
			pBody->IsAgainstWall(VannoEngine::Direction::DOWN),
			pBody->IsAgainstWall(VannoEngine::Direction::UP),
			pBody->IsAgainstWall(VannoEngine::Direction::RIGHT),
			pBody->IsAgainstWall(VannoEngine::Direction::LEFT)
			);
	}*/
}

void ArrowController::HandleEvent(std::string eventName, VannoEngine::Event* event) {
	if (event->GetName() == EVT_MAP_COLLISION) {
		VannoEngine::MapCollisionEvent* pCollisionEvent = dynamic_cast<VannoEngine::MapCollisionEvent*>(event);
		VannoEngine::PhysicsBody* pBody = dynamic_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));

		if (pCollisionEvent->GetBody() == pBody) {
			VannoEngine::Transform* pTransform = dynamic_cast<VannoEngine::Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));

			pBody->SetMass(0.0f);
			pTransform->SetSpeed(0.0f, 0.0f);
		}
	}
}
