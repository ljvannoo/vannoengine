#include "LightBanditController.h"

#include "DamageEvent.h"
#include "DeathEvent.h"
#include "TurnAroundEvent.h"
#include "DetectedEnemyEvent.h"

#include "engine/components/Transform.h"
#include "engine/components/Sprite.h"
#include "engine/components/PhysicsBody.h"
#include "engine/components/Animator.h"

#include "engine/systems/events/EventManager.h"

#include "engine/components/PhysicsBody.h"
#include "engine/components/Transform.h"
#include "engine/systems/physics/MapCollisionEvent.h"
#include "engine/systems/physics/ObjectCollisionEvent.h"

#include "engine/systems/objects/GameObjectFactory.h"

#include "engine/util/Directions.h"

#include "engine/core/Log.h"

#include <glm/gtx/vector_angle.hpp>

LightBanditController::LightBanditController(VannoEngine::GameObject* owner) :
	GameComponent(owner),
	mCurrentState{ State::Idle },
	mCooldown{ 0.0f },
	mAlertCooldown{ cAlertCooldown }
{
	VannoEngine::EventManager::GetInstance()->Subscribe(EVT_OBJECT_COLLISION, this);
	VannoEngine::EventManager::GetInstance()->Subscribe(EVT_MAP_COLLISION, this);
}

LightBanditController::~LightBanditController() {
	VannoEngine::EventManager::GetInstance()->Unsubscribe(EVT_OBJECT_COLLISION, this);
	VannoEngine::EventManager::GetInstance()->Unsubscribe(EVT_MAP_COLLISION, this);
}


void LightBanditController::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
}

void LightBanditController::Update(double deltaTime) {
	VannoEngine::Transform* pTransform = dynamic_cast<VannoEngine::Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));
	VannoEngine::PhysicsBody* pBody = dynamic_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));
	VannoEngine::Animator* pAnimator = dynamic_cast<VannoEngine::Animator*>(GetOwner()->GetComponent(ANIMATOR_COMPONENT));

	glm::vec2 speed = pTransform->GetSpeed();
	float targetSpeed = 0.0f;
	switch (mCurrentState) {
	case State::Idle:
		pAnimator->Play("idle");
		speed.x = 0.0f;
		break;
	case State::Alert:
		pAnimator->Play("combat_idle");
		speed.x = 0.0f;
		mAlertCooldown -= deltaTime;
		if (mAlertCooldown <= 0.0) {
			mCurrentState = State::Idle;
		}
		break;
	case State::Dieing:
		pAnimator->Play("dieing");
		targetSpeed = 0.0f;
		mCooldown -= deltaTime;
		if (mCooldown <= 0.0) {
			mCurrentState = State::Dead;
			mCooldown = 3.0;
		}
		break;
	case State::Dead:
		pAnimator->Play("dead");
		targetSpeed = 0.0f;
		mCooldown -= deltaTime;
		if (mCooldown <= 0.0) {
			GetOwner()->Destroy();
		}
		break;
	}

	if (speed.y < 0.0f) {
		TurnAround();
	}

	speed.x = MoveTowards(speed.x, targetSpeed * mDirection, cWalkAccel * (float)deltaTime);
	pTransform->SetSpeed(speed.x, speed.y);

	if (mCooldown > 0.0) {
		mCooldown -= deltaTime;
	}
}

void LightBanditController::Draw() {

}

void LightBanditController::HandleEvent(std::string eventName, VannoEngine::Event* event) {
	if (mCurrentState < State::Dieing) {
		/*if (event->GetName() == EVT_OBJECT_COLLISION) {
			VannoEngine::ObjectCollisionEvent* pCollisionEvent = dynamic_cast<VannoEngine::ObjectCollisionEvent*>(event);
			VannoEngine::PhysicsBody* pBody = dynamic_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));
			VannoEngine::PhysicsBody* pOtherBody = pCollisionEvent->GetOtherBody();

			if (pCollisionEvent->GetBody() == pBody) {
				if (pOtherBody->GetPhysicsLayer() == "player" && mCooldown <= 0.0) {
					DamageEvent* pEvent = new DamageEvent(GetOwner(), pOtherBody->GetOwner(), 10.0f);
					VannoEngine::EventManager::GetInstance()->Direct(pOtherBody->GetOwner(), pEvent);
					mCooldown = cDamageCooldown;
				}
			}
		}*/
		/*if (event->GetName() == EVT_MAP_COLLISION) {
			VannoEngine::MapCollisionEvent* pCollisionEvent = dynamic_cast<VannoEngine::MapCollisionEvent*>(event);
			VannoEngine::PhysicsBody* pBody = dynamic_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));

			if (pBody == pCollisionEvent->GetBody()) {
				switch (pCollisionEvent->GetDirection()) {
				case VannoEngine::Direction::RIGHT:
				case VannoEngine::Direction::LEFT:
					TurnAround();
					break;
				}
			}
		}*/
	}
}

void LightBanditController::HandleLocalEvent(std::string eventName, VannoEngine::Event* event) {
	if (event->GetName() == EVT_DEATH) {
		mCurrentState = State::Dieing;
		mCooldown = 0.8;
	}
	else if (event->GetName() == EVT_DETECTED_ENEMY) {
		DetectedEnemyEvent* pEvent = dynamic_cast<DetectedEnemyEvent*>(event);

		VannoEngine::Sprite* pSprite = dynamic_cast<VannoEngine::Sprite*>(GetOwner()->GetComponent(SPRITE_COMPONENT));
		VannoEngine::Transform* pTransform = dynamic_cast<VannoEngine::Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));
		VannoEngine::Transform* pEnemyTransform = dynamic_cast<VannoEngine::Transform*>(pEvent->GetEnemy()->GetComponent(TRANSFORM_COMPONENT));
		if (pEnemyTransform->GetPosition().x < pTransform->GetPosition().x && !pSprite->IsHorizontalFlipped()) {
			VannoEngine::EventManager::GetInstance()->Direct(GetOwner(), new TurnAroundEvent(GetOwner()));
		} else if (pEnemyTransform->GetPosition().x > pTransform->GetPosition().x && pSprite->IsHorizontalFlipped()) {
			VannoEngine::EventManager::GetInstance()->Direct(GetOwner(), new TurnAroundEvent(GetOwner()));
		}
		mCurrentState = State::Alert;
		mAlertCooldown = cAlertCooldown;
	}
	else if (event->GetName() == EVT_TURN_AROUND) {
		TurnAround();
	}
}

float LightBanditController::MoveTowards(float current, float target, float maxDelta) {
	if (fabs(target - current) <= maxDelta) {
		return target;
	}
	float sign = 1.0f;
	if (target - current < 0.0f) {
		sign = -1.0f;
	}
	return current + sign * maxDelta;
}

void LightBanditController::TurnAround() {
	VannoEngine::Sprite* pSprite = dynamic_cast<VannoEngine::Sprite*>(GetOwner()->GetComponent(SPRITE_COMPONENT));
	VannoEngine::Transform* pTransform = dynamic_cast<VannoEngine::Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));

	mDirection *= -1.0f;
	glm::vec2 pos = pTransform->GetPosition();
	pos.x += 5.0f * mDirection;
	pTransform->SetPositionX(pos.x);
	pSprite->FlipHorizontal();
}
