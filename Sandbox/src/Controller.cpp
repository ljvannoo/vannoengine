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
#include "engine/systems/FramerateController.h"
#include "engine/systems/InputManager.h"
#include "engine/systems/ConfigurationManager.h"

#include "engine/systems/physics/PhysicsManager.h"
#include "engine/systems/physics/Collision.h"
#include "engine/systems/physics/Aabb.h"

#include "engine/systems/levels/LevelManager.h"
#include "engine/systems/levels/Level.h"

#include "Actions.h"

#include <algorithm>

Controller::Controller(VannoEngine::GameObject* owner) :
	GameComponent(owner),
	mCurrentState{ State::Stand },
	mpInputManager{ VannoEngine::InputManager::GetInstance() },
	mpPhysicsManager{ VannoEngine::PhysicsManager::GetInstance() },
	mpConfigManager{ VannoEngine::ConfigurationManager::GetInstance() },
	mpLevelManager{ VannoEngine::LevelManager::GetInstance() }
{}

Controller::~Controller() {
}

void Controller::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {

}

void Controller::Update(double deltaTime) {
	VannoEngine::Sprite* pSprite = dynamic_cast<VannoEngine::Sprite*>(GetOwner()->GetComponent(SPRITE_COMPONENT));
	VannoEngine::Transform* pTransform = dynamic_cast<VannoEngine::Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));
	VannoEngine::PhysicsBody* pBody = dynamic_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));
	VannoEngine::Animator* pAnimator = dynamic_cast<VannoEngine::Animator*>(GetOwner()->GetComponent(ANIMATOR_COMPONENT));

	HandleCollisions(pTransform, pBody);

	if (mpInputManager->IsKeyTriggered(ACTION_DEBUG)) {
		mpConfigManager->ToggleBool("/debugMode");
		LOG_CORE_DEBUG("Debug Mode is now: {0}", mpConfigManager->GetBool("/debugMode"));
	}

	VannoEngine::Collision collision = pBody->GetCollision();
	glm::vec2 speed = pTransform->GetSpeed();
	switch (mCurrentState) {
	case State::Stand:
		pAnimator->Play("idle");
		speed.x = 0.0f;
		if(speed.x == 0) {
			if (mpInputManager->IsKeyPressed(ACTION_DOWN)) {
				mCurrentState = State::Crouch;
			}
			else if (mpInputManager->IsKeyPressed(ACTION_RIGHT)) {
				mCurrentState = State::Walk;
				pSprite->SetFlipHorizontal(false);
				speed.x += 11.0f;
			}
			else if (mpInputManager->IsKeyPressed(ACTION_LEFT)) {
				mCurrentState = State::Walk;
				pSprite->SetFlipHorizontal(true);
				speed.x -= 11.0f;
			}

			if (mpInputManager->IsKeyPressed(ACTION_JUMP)) {
				mCurrentState = State::Jump;
				speed.y += 100.0f;
				pTransform->SetPositionY(pTransform->GetPosition().y + 10.0f);
			}
		}
		break;
	case State::Crouch:
		pAnimator->Play("crouch");

		if (!mpInputManager->IsKeyPressed(ACTION_DOWN)) {
			mCurrentState = State::Stand;
		}
		break;
	case State::Walk:
		pAnimator->Play("walk");
		speed.x *= 1.0f + cWalkAccel;
		if (speed.x > cWalkSpeed) {
			speed.x = cWalkSpeed;
		}
		else if (speed.x < -cWalkSpeed) {
			speed.x = -cWalkSpeed;
		}
		if(mpInputManager->IsKeyReleased(ACTION_RIGHT) || mpInputManager->IsKeyReleased(ACTION_LEFT)) {
			mCurrentState = State::Slowing;	
		}
		if (mpInputManager->IsKeyPressed(ACTION_JUMP)) {
			mCurrentState = State::Jump;
			speed.y += 200.0f;
			pTransform->SetPositionY(pTransform->GetPosition().y + 10.0f);
		}
		break;
	case State::Slowing:
		//pAnimator->Play("idle");
		speed.x *= cWalkAccel;
		if (speed.x > -10.0f && speed.x < 10.0f) {
			speed.x = 0.0f;
			mCurrentState = State::Stand;
		}

		break;
	case State::Jump:
		pAnimator->Play("jump");
		if (mpInputManager->IsKeyPressed(ACTION_JUMP) && speed.y < cJumpSpeed) {
			speed.y += 70.0f;
			if (speed.y > cJumpSpeed) {
				//speed.y = -1.0f;
				mCurrentState = State::Falling;

			}
		}
		
		if (mpInputManager->IsKeyReleased(ACTION_RIGHT) || mpInputManager->IsKeyReleased(ACTION_LEFT)) {
			mCurrentState = State::Slowing;
		}

		if (collision.CollisionDetected(VannoEngine::Direction::BOTTOM)) {
			mCurrentState = State::Stand;
			speed.y = 0.0f;
		}
		break;
	case State::Falling:
		if(speed.y < 0.0) {
			pAnimator->Play("falling");
		}
		if(collision.CollisionDetected(VannoEngine::Direction::BOTTOM)) {
			pAnimator->Play("walk");
			if (mpInputManager->IsKeyPressed(ACTION_RIGHT) || mpInputManager->IsKeyPressed(ACTION_LEFT)) {
				mCurrentState = State::Walk;
			}
			else {
				mCurrentState = State::Slowing;
			}
		}
		break;
	}
	LOG_DEBUG("SpeedY: {}, State: {}", speed.y, mCurrentState);
	pTransform->SetSpeed(speed.x, speed.y);
	UpdateCamera(pTransform);
}

void Controller::Jump() {

}

void Controller::Draw() {

}
void Controller::HandleCollisions(VannoEngine::Transform* pTransform, VannoEngine::PhysicsBody* pBody) {
	VannoEngine::Collision collision = pBody->GetCollision();
	VannoEngine::AABB aabb = pBody->GetAabb();
	glm::vec2 aabbOffset = pBody->GetAabbOffset();

	glm::vec2 newPosition = pTransform->GetPosition();
	glm::vec2 newSpeed = pTransform->GetSpeed();
	if (collision.CollisionDetected(VannoEngine::Direction::BOTTOM) && pTransform->GetSpeed().y < 0.0f) {
		newPosition.y = collision.GetEdge(VannoEngine::Direction::BOTTOM) + aabb.halfHeight - aabbOffset.y;
		newSpeed.y = 0.0f;
	} else if (collision.CollisionDetected(VannoEngine::Direction::TOP) && pTransform->GetSpeed().y > 0.0f) {
		newPosition.y = collision.GetEdge(VannoEngine::Direction::TOP) - aabb.halfHeight - aabbOffset.y;
		newSpeed.y = 0.0f;
	}

	if (collision.CollisionDetected(VannoEngine::Direction::LEFT) && pTransform->GetSpeed().x < 0.0f) {
		newPosition.x = collision.GetEdge(VannoEngine::Direction::LEFT) + aabb.halfWidth - aabbOffset.x;
		newSpeed.x = 0.0f;
	} else if (collision.CollisionDetected(VannoEngine::Direction::RIGHT) && pTransform->GetSpeed().x > 0.0f) {
		newPosition.x = collision.GetEdge(VannoEngine::Direction::RIGHT) - aabb.halfWidth - aabbOffset.x;
		newSpeed.x = 0.0f;
	}

	pTransform->SetPosition(newPosition.x, newPosition.y);
	pTransform->SetSpeed(newSpeed.x, newSpeed.y);
}

void Controller::UpdateCamera(VannoEngine::Transform* pTransform) {
	VannoEngine::Camera* pCamera = mpLevelManager->GetCamera();
	VannoEngine::Level* pLevel = mpLevelManager->GetCurrentLevel();
	float levelWidth = pLevel->GetWidth();
	float levelHeight = pLevel->GetHeight();
	float hW = pCamera->GetScreenWidth() / 2.0f;
	float hH = pCamera->GetScreenHeight() / 2.0f;

	glm::vec2 cameraPosition = pTransform->GetPosition();
	if (cameraPosition.x - hW < 0.0f) {
		cameraPosition.x = hW;
	}
	else if (cameraPosition.x + hW > levelWidth) {
		cameraPosition.x = levelWidth - hW;
	}

	/*if (cameraPosition.y - hH < 0.0f) {
		cameraPosition.y = hH;
	}
	else if (cameraPosition.y + hH > levelHeight) {
		cameraPosition.y = levelHeight - hH;
	}*/
	cameraPosition.y = hH;


	pCamera->SetPosition(cameraPosition);

}
