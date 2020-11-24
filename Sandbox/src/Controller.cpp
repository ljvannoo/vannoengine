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
#include "engine/components/Animator.h"
#include "engine/components/Sprite.h"
#include "engine/components/PhysicsBody.h"
#include "engine/components/Animator.h"
#include "engine/components/Camera.h"

#include "engine/systems/objects/GameObject.h"

#include "engine/systems/events/EventManager.h"
#include "engine/systems/events/Event.h"
#include "engine/systems/physics/MapCollisionEvent.h"

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
	mOnGround{ false },
	mAtCeiling{ false },
	mAgainstLeftWall{ false },
	mAgainstRightWall{ false }
{
	VannoEngine::EventManager::GetInstance()->Subscribe(EVT_MAP_COLLISION, this);
}

Controller::~Controller() {
}

void Controller::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {

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

	float targetSpeed = 0.0f;
	switch (mCurrentState) {
	case State::Stand:
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
		if ((mpInputManager->IsKeyPressed(ACTION_RIGHT) && mAgainstRightWall) ||
			(mpInputManager->IsKeyPressed(ACTION_LEFT) && mAgainstLeftWall)) {
			mCurrentState = State::Stand;
				targetSpeed = 0.0f;
		}
		if (mpInputManager->IsKeyPressed(ACTION_JUMP)) {
			mCurrentState = State::Jump;
		}
		if (mpInputManager->IsKeyPressed(ACTION_DOWN)) {
			mCurrentState = State::Crouch;
		}
		if (mpInputManager->IsKeyPressed(ACTION_ATTACK)) {
			mCurrentState = State::Attack;
			mAttackStartTime = mpTimeManager->GetElapsedMillis();
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
		std::stringstream ss;
		ss << "punch" << mAttackNum;
		pAnimator->Play(ss.str());
		targetSpeed = 0.0f;
		if (mpTimeManager->GetElapsedMillis() > mAttackStartTime + cAttackDuration) {
			if (mpInputManager->IsKeyPressed(ACTION_ATTACK) && mAttackNum < 3) {
				mAttackStartTime = mpTimeManager->GetElapsedMillis();
				mAttackNum++;
			}
			else {
				mCurrentState = State::Stand;
				mAttackNum = 1;
			}
		}
		break;
	}

	//LOG_DEBUG("({}) Targetspeed: {}", mCurrentState, targetSpeed);
	speed.x = MoveTowards(speed.x, targetSpeed * moveInput, cWalkAccel * (float)deltaTime);
	//LOG_DEBUG("SpeedY: {}, State: {}", speed.y, mCurrentState);
	pTransform->SetSpeed(speed.x, speed.y);
	UpdateCamera(pTransform);
	
	mOnGround = false;
	mAtCeiling = false;
	mAgainstLeftWall = false;
	mAgainstRightWall = false;
	
}

void Controller::Jump() {

}

void Controller::Draw() {

}

void Controller::HandleEvent(std::string eventName, VannoEngine::Event* event) {
	if (event->GetName() == EVT_MAP_COLLISION) {
		VannoEngine::MapCollisionEvent* pCollisionEvent = dynamic_cast<VannoEngine::MapCollisionEvent*>(event);

		VannoEngine::PhysicsBody* pBody = pCollisionEvent->GetBody();
		VannoEngine::AABB aabb = pBody->GetAabb();
		glm::vec2 aabbOffset = pBody->GetAabbOffset();

		VannoEngine::Transform* pTransform = dynamic_cast<VannoEngine::Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));
		glm::vec2 newPosition = pTransform->GetPosition();
		glm::vec2 newSpeed = pTransform->GetSpeed();

		switch (pCollisionEvent->GetDirection()) {
		case VannoEngine::Direction::DOWN:
			if (newSpeed.y < 0.0f) {
				if(pCollisionEvent->GetType() == VannoEngine::CollisionType::HARD || !mpInputManager->IsKeyPressed(ACTION_DOWN)) {
					newPosition.y = pCollisionEvent->GetPlane() + aabb.halfHeight - aabbOffset.y;
					newSpeed.y = 0.0f;
					mOnGround = true;
				}
				else {
					mCurrentState = State::Fall;
				}
			}
			break;
		case VannoEngine::Direction::UP:
			if (newSpeed.y > 0.0f && pCollisionEvent->GetType() == VannoEngine::CollisionType::HARD) {
				newPosition.y = pCollisionEvent->GetPlane() - aabb.halfHeight - aabbOffset.y;
				newSpeed.y = 0.0f;
				mAtCeiling = true;
			}
			break;
		case VannoEngine::Direction::LEFT:
			if (newSpeed.x < 0.0f && pCollisionEvent->GetType() == VannoEngine::CollisionType::HARD) {
				newPosition.x = pCollisionEvent->GetPlane() + aabb.halfWidth - aabbOffset.x;
				newSpeed.x = 0.0f;
				mAgainstLeftWall = true;
			}
			break;
		case VannoEngine::Direction::RIGHT:
			if (newSpeed.x > 0.0f && pCollisionEvent->GetType() == VannoEngine::CollisionType::HARD) {
				newPosition.x = pCollisionEvent->GetPlane() - aabb.halfWidth - aabbOffset.x;
				newSpeed.x = 0.0f;
				mAgainstRightWall = true;
			}
			break;
		}

		pTransform->SetPosition(newPosition.x, newPosition.y);
		pTransform->SetSpeed(newSpeed.x, newSpeed.y);
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