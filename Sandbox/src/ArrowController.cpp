/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		ArrowController.cpp
Purpose:		Defines the behavior of arrows fired by the player
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-27
*************************************************************************/

#include "ArrowController.h"

#include "engine/systems/events/DamageEvent.h"

#include "engine/systems/events/EventManager.h"

#include "engine/components/PhysicsBody.h"
#include "engine/components/Transform.h"
#include "engine/systems/physics/MapCollisionEvent.h"
#include "engine/systems/physics/ObjectCollisionEvent.h"

#include "HealthTracker.h"
#include "engine/core/Log.h"

#include <glm/gtx/vector_angle.hpp>

ArrowController::ArrowController(VannoEngine::GameObject* owner) :
	GameComponent(owner),
	mTimeToLive{ 3.0 }
{
	VannoEngine::EventManager::GetInstance()->Subscribe(EVT_MAP_COLLISION, this);
	VannoEngine::EventManager::GetInstance()->Subscribe(EVT_OBJECT_COLLISION, this);
}

ArrowController::~ArrowController() {
	VannoEngine::EventManager::GetInstance()->Unsubscribe(EVT_MAP_COLLISION, this);
	VannoEngine::EventManager::GetInstance()->Unsubscribe(EVT_OBJECT_COLLISION, this);
}


void ArrowController::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
	if (pData->HasMember("damage") && (*pData)["damage"].IsNumber()) {
		mDamage = (*pData)["damage"].GetFloat();
	}
}

void ArrowController::Update(double deltaTime) {
	VannoEngine::Transform* pTransform = dynamic_cast<VannoEngine::Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));
	glm::vec2 forward(1.0f, 0.0f);
	glm::vec2 speed = pTransform->GetSpeed();
	speed = glm::normalize(speed);
	if (glm::abs(glm::length(speed)) > 0.0f) {
		pTransform->SetRotation(glm::degrees(glm::angle(speed, forward)));
	}
	else {
		mTimeToLive -= deltaTime;
		if (mTimeToLive <= 0.0) {
			GetOwner()->Destroy();
		}
	}
}

void ArrowController::Draw() {

}

void ArrowController::HandleEvent(std::string eventName, VannoEngine::Event* event) {
	if (event->GetName() == EVT_MAP_COLLISION) {
		VannoEngine::MapCollisionEvent* pCollisionEvent = dynamic_cast<VannoEngine::MapCollisionEvent*>(event);
		VannoEngine::PhysicsBody* pBody = dynamic_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));

		if (pCollisionEvent->GetBody() == pBody && pCollisionEvent->GetType() == VannoEngine::CollisionType::HARD) {
			VannoEngine::Transform* pTransform = dynamic_cast<VannoEngine::Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));

			pBody->SetMass(0.0f);
			pTransform->SetSpeed(0.0f, 0.0f);
		}
	} else if (event->GetName() == EVT_OBJECT_COLLISION) {
		VannoEngine::ObjectCollisionEvent* pCollisionEvent = dynamic_cast<VannoEngine::ObjectCollisionEvent*>(event);
		VannoEngine::PhysicsBody* pBody = dynamic_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));

		if (pCollisionEvent->GetBody() == pBody && pCollisionEvent->GetOtherBody()->GetPhysicsLayer() == "enemy") {
			HealthTracker* pEnemyHealth = dynamic_cast<HealthTracker*>(pCollisionEvent->GetOtherBody()->GetOwner()->GetComponent(HEALTH_TRACKER_COMPONENT));
			if(pEnemyHealth->IsAlive()) {
				VannoEngine::GameObject* pSource = GetOwner();
				VannoEngine::GameObject* pTarget = pCollisionEvent->GetOtherBody()->GetOwner();
				VannoEngine::EventManager::GetInstance()->Direct(pTarget, new VannoEngine::DamageEvent(pSource, pTarget, mDamage));
				GetOwner()->Destroy();
			}
		}
	}
}
