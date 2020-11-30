/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Controller.cpp
Purpose:		Implementation of the Controller component
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-19
*************************************************************************/

#include "Controller.h"

#include "engine/core/Log.h"

#include "engine/components/Transform.h"
#include "engine/components/Sprite.h"
#include "engine/components/PhysicsBody.h"
#include "engine/components/Animator.h"
#include "engine/components/Camera.h"

#include "engine/systems/objects/GameObjectFactory.h"
#include "engine/systems/objects/GameObject.h"

#include "engine/systems/events/EventManager.h"
#include "engine/systems/events/Event.h"
#include "engine/systems/physics/ObjectCollisionEvent.h"

#include "engine/systems/FramerateController.h"
#include "engine/systems/InputManager.h"
#include "engine/systems/ConfigurationManager.h"

#include "engine/systems/physics/PhysicsManager.h"
#include "engine/systems/physics/Aabb.h"

#include "engine/systems/levels/LevelManager.h"
#include "engine/systems/levels/Level.h"

#include "engine/systems/TimeManager.h"

#include "Actions.h"
#include "engine/util/Directions.h"

#include "PowerUp.h"
#include "DamageEvent.h"
#include "InvulnerableEvent.h"

#include <algorithm>
#include <sstream>

Controller::Controller(VannoEngine::GameObject* owner) :
	GameComponent(owner),
	mCurrentState{ State::Stand },
	mpInputManager{ VannoEngine::InputManager::GetInstance() },
	mpPhysicsManager{ VannoEngine::PhysicsManager::GetInstance() },
	mpConfigManager{ VannoEngine::ConfigurationManager::GetInstance() },
	mpLevelManager{ VannoEngine::LevelManager::GetInstance() },
	mpTimeManager{ VannoEngine::TimeManager::GetInstance() },
	mHasSword{ false },
	mHasBow{ false },
	mAttackStartTime{ 0 },
	mCanDoDamage{ false },
	mFistDamage{ 0.0f },
	mSwordDamage{ 0.0f }
{
	VannoEngine::EventManager::GetInstance()->Subscribe(EVT_OBJECT_COLLISION, this);
}

Controller::~Controller() {
	VannoEngine::EventManager::GetInstance()->Unsubscribe(EVT_OBJECT_COLLISION, this);
}

void Controller::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
	if (pData->HasMember("fistDamage") && (*pData)["fistDamage"].IsNumber()) {
		mFistDamage = (*pData)["fistDamage"].GetFloat();
	}
	if (pData->HasMember("swordDamage") && (*pData)["swordDamage"].IsNumber()) {
		mSwordDamage = (*pData)["swordDamage"].GetFloat();
	}
}

void Controller::Update(double deltaTime) {
	VannoEngine::Sprite* pSprite = dynamic_cast<VannoEngine::Sprite*>(GetOwner()->GetComponent(SPRITE_COMPONENT));
	VannoEngine::Transform* pTransform = dynamic_cast<VannoEngine::Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));
	VannoEngine::PhysicsBody* pBody = dynamic_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));
	VannoEngine::Animator* pAnimator = dynamic_cast<VannoEngine::Animator*>(GetOwner()->GetComponent(ANIMATOR_COMPONENT));

	glm::vec2 speed = pTransform->GetSpeed();

	float moveInput = 0.0f;

	if (mpInputManager->IsKeyPressed(ACTION_RIGHT)) {
		pSprite->SetFlipHorizontal(false);
		
		moveInput = 1.0f;
	}
	if (mpInputManager->IsKeyPressed(ACTION_LEFT)) {
		pSprite->SetFlipHorizontal(true);
		moveInput = -1.0f;
	}
	std::stringstream ss;
	float targetSpeed = 0.0f;
	switch (mCurrentState) {
	case State::Stand:
		mCanDoDamage = false;
		pAnimator->Play("idle");
		speed.x = 0.0f;
		if (mpInputManager->IsKeyPressed(ACTION_WALK)) {
			if (mpInputManager->IsKeyPressed(ACTION_RIGHT) || mpInputManager->IsKeyPressed(ACTION_LEFT)) {
				mCurrentState = State::Walk;
				targetSpeed = cWalkSpeed;
			}
		}
		else if (mpInputManager->IsKeyPressed(ACTION_DASH)) {
			if (mpInputManager->IsKeyPressed(ACTION_RIGHT) || mpInputManager->IsKeyPressed(ACTION_LEFT)) {
				mCurrentState = State::Dash;
				targetSpeed = cDashSpeed;
			}
		}
		else {
			if (mpInputManager->IsKeyPressed(ACTION_RIGHT) || mpInputManager->IsKeyPressed(ACTION_LEFT)) {
				mCurrentState = State::Run;
				targetSpeed = cRunSpeed;
			}
		}
		/*if ((mpInputManager->IsKeyPressed(ACTION_RIGHT) && mAgainstRightWall) ||
			(mpInputManager->IsKeyPressed(ACTION_LEFT) && mAgainstLeftWall)) {
			mCurrentState = State::Stand;
				targetSpeed = 0.0f;
		}*/
		if (mpInputManager->IsKeyPressed(ACTION_JUMP)) {
			mCurrentState = State::Jump;
		}
		if (mpInputManager->IsKeyPressed(ACTION_DOWN)) {
			mCurrentState = State::Crouch;
			pBody->FallThroughFloor();
		}
		if (mpInputManager->IsKeyPressed(ACTION_ATTACK)) {
			mCurrentState = State::Attack;
			mAttackStartTime = mpTimeManager->GetElapsedMillis();
			if (mHasSword) {
				mAttackDuration = 600l;
			}
			else {
				mAttackDuration = 400l;
			}

			VannoEngine::EventManager::GetInstance()->Direct(GetOwner(), new InvulnerableEvent(GetOwner(), true));
		}
		if (mpInputManager->IsKeyPressed(ACTION_FIRE) && mHasBow) {
			mCurrentState = State::Shoot;
			mAttackStartTime = mpTimeManager->GetElapsedMillis();
			mAttackDuration = 600l;
		}
		break;
	case State::Walk:
		pAnimator->Play("walk");
		targetSpeed = cWalkSpeed;
		if (!mpInputManager->IsKeyPressed(ACTION_WALK)) {
			mCurrentState = State::Run;
			targetSpeed = cRunSpeed;
		}
		if (mpInputManager->IsKeyPressed(ACTION_JUMP)) {
			mCurrentState = State::Jump;
		}
		if (mpInputManager->IsKeyPressed(ACTION_DOWN)) {
			pBody->FallThroughFloor();
		}
		break;
	case State::Run:
		pAnimator->Play("run");
		targetSpeed = cRunSpeed;
		if (mpInputManager->IsKeyPressed(ACTION_WALK)) {
			mCurrentState = State::Walk;
			targetSpeed = cWalkSpeed;
		}
		if (mpInputManager->IsKeyPressed(ACTION_DASH)) {
			mCurrentState = State::Dash;
			targetSpeed = cDashSpeed;
		}
		if (mpInputManager->IsKeyPressed(ACTION_JUMP)) {
			mCurrentState = State::Jump;
		}
		if (mpInputManager->IsKeyPressed(ACTION_DOWN)) {
			pBody->FallThroughFloor();
		}
		if (speed.x == 0.0f) {
			mCurrentState = State::Stand;
		}
		break;
	case State::Dash:
		pAnimator->Play("dash");
		targetSpeed = cDashSpeed;
		if (!mpInputManager->IsKeyPressed(ACTION_DASH)) {
			mCurrentState = State::Run;
			targetSpeed = cRunSpeed;
		}
		if (mpInputManager->IsKeyPressed(ACTION_JUMP)) {
			mCurrentState = State::Jump;
		}
		if (mpInputManager->IsKeyPressed(ACTION_DOWN)) {
			pBody->FallThroughFloor();
		}
		break;
	case State::Jump:
		pAnimator->Play("jump");
		targetSpeed = cRunSpeed;
		if (mpInputManager->IsKeyPressed(ACTION_JUMP)) {
			mCurrentState = State::Jump;
			speed.y = sqrtf(2.0f * cJumpHeight * fabsf(mpPhysicsManager->GetGravity())) ;
			mCurrentState = State::Jumping;
		}
		
		break;
	case State::Jumping:
		targetSpeed = cRunSpeed;
		if (speed.y <= 0.0f) {
			mCurrentState = State::Fall;
		}
		break;
	case State::Fall:
		pAnimator->Play("falling");
		targetSpeed = cRunSpeed;
		if (speed.y == 0.0f) {
			if (speed.x != 0.0f) {
				mCurrentState = State::Run;
			}
			else {
				mCurrentState = State::Stand;
			}
		}
		break;
	case State::Crouch:
		pAnimator->Play("crouch");
		targetSpeed = 0.0f;
		if (mpInputManager->IsKeyReleased(ACTION_DOWN)) {
			mCurrentState = State::Stand;
		}
		break;
	case State::Attack:
		if(mHasSword) {
			ss << "sword" << mAttackNum;
		}
		else {
			ss << "punch" << mAttackNum;
		}
		pAnimator->Play(ss.str());
		targetSpeed = 0.0f;
		if (mpTimeManager->GetElapsedMillis() > mAttackStartTime + mAttackDuration) {
			if (mpInputManager->IsKeyPressed(ACTION_ATTACK) && mAttackNum < 3) {
				mAttackStartTime = mpTimeManager->GetElapsedMillis();
				mAttackNum++;
			}
			else {
				mCanDoDamage = true;
				mCurrentState = State::Stand;
				mAttackNum = 1;
				VannoEngine::EventManager::GetInstance()->Direct(GetOwner(), new InvulnerableEvent(GetOwner(), false));
			}
		}
		break;
	case State::Shoot:
		pAnimator->Play("shoot");
		targetSpeed = 0.0f;
		if (mpTimeManager->GetElapsedMillis() > mAttackStartTime + mAttackDuration) {
			VannoEngine::GameObjectFactory* pFactory = VannoEngine::GameObjectFactory::GetInstance();
			VannoEngine::GameObject* pArrow = pFactory->CreateObject("objects\\arrow1.json", GetOwner()->GetMapLayer());
			VannoEngine::Sprite* pArrowSprite = dynamic_cast<VannoEngine::Sprite*>(pArrow->GetComponent(SPRITE_COMPONENT));
			VannoEngine::Transform* pArrowTransform = dynamic_cast<VannoEngine::Transform*>(pArrow->GetComponent(TRANSFORM_COMPONENT));
			glm::vec2 pos = pBody->GetAabbCenter();
			
			pArrowTransform->SetPosition(pos.x, pos.y);
			if (pSprite->IsHorizontalFlipped()) {
				pArrowTransform->SetSpeedX(pArrowTransform->GetSpeed().x * -1.0f);
				pArrowSprite->SetFlipHorizontal(false);
			}

			mCurrentState = State::Stand;
		}
		break;
	}

	//LOG_DEBUG("({}) Targetspeed: {}", mCurrentState, targetSpeed);
	speed.x = MoveTowards(speed.x, targetSpeed * moveInput, cWalkAccel * (float)deltaTime);
	//LOG_DEBUG("SpeedY: {}, State: {}", speed.y, mCurrentState);
	pTransform->SetSpeed(speed.x, speed.y);
	UpdateCamera(pTransform);

	if (speed.y < 0.0f) {
		mCurrentState = State::Fall;
	}
}

void Controller::Jump() {

}

void Controller::Draw() {

}

void Controller::HandleEvent(std::string eventName, VannoEngine::Event* event) {
	if (event->GetName() == EVT_OBJECT_COLLISION) {
		VannoEngine::ObjectCollisionEvent* pCollisionEvent = dynamic_cast<VannoEngine::ObjectCollisionEvent*>(event);
		VannoEngine::PhysicsBody* pBody = dynamic_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));
		VannoEngine::PhysicsBody* pOtherBody = pCollisionEvent->GetOtherBody();
		if(pCollisionEvent->GetBody() == pBody) {
			VannoEngine::GameObject* pObject = pCollisionEvent->GetOtherBody()->GetOwner();

			if (pObject->HasComponent(POWERUP_COMPONENT)) {
				if(pObject->GetName() == "sword") {
					mHasSword = true;
				} else if (pObject->GetName() == "bow") {
					mHasBow = true;
				}
				pObject->Destroy();
			} else if(pOtherBody->GetPhysicsLayer() == "enemy" && mCanDoDamage) {
				float damage = mFistDamage;
				if (mHasSword) {
					damage = mSwordDamage;
				}
				DamageEvent* pEvent = new DamageEvent(GetOwner(), pOtherBody->GetOwner(), damage);
				VannoEngine::EventManager::GetInstance()->Direct(pOtherBody->GetOwner(), pEvent);
				mCanDoDamage = false;
			}
		}
	}
}

void Controller::UpdateCamera(VannoEngine::Transform* pTransform) {
	VannoEngine::Camera* pCamera = mpLevelManager->GetCamera();
	VannoEngine::Level* pLevel = mpLevelManager->GetCurrentLevel();
	float levelWidth = pLevel->GetWidth();
	float levelHeight = pLevel->GetHeight();
	float hW = pCamera->GetScaledWidth() / 2.0f;
	float hH = pCamera->GetScaledHeight() / 2.0f;

	glm::vec2 cameraPosition = pTransform->GetPosition();
	if (cameraPosition.x - hW < 0.0f) {
		cameraPosition.x = hW;
	}
	else if (cameraPosition.x + hW > levelWidth) {
		cameraPosition.x = levelWidth - hW;
	}

	if (cameraPosition.y - hH < 0.0f) {
		cameraPosition.y = hH;
	}
	else if (cameraPosition.y + hH > levelHeight) {
		cameraPosition.y = levelHeight - hH;
	}
	//cameraPosition.y = hH;


	pCamera->SetPosition(cameraPosition);

}

float Controller::MoveTowards(float current, float target, float maxDelta) {
	if (fabs(target - current) <= maxDelta) {
		return target;
	}
	float sign = 1.0f;
	if (target - current < 0.0f) {
		sign = -1.0f;
	}
	return current + sign * maxDelta;
}