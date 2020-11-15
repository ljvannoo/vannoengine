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
	mCurrentState(State::Stand)
{}

Controller::~Controller() {
}

void Controller::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
	
}

void Controller::Update(double deltaTime) {
	VannoEngine::InputManager* pInputManager = VannoEngine::InputManager::GetInstance();
	VannoEngine::PhysicsManager* pPhysicsManager = VannoEngine::PhysicsManager::GetInstance();

	VannoEngine::Sprite* pSprite = dynamic_cast<VannoEngine::Sprite*>(GetOwner()->GetComponent(SPRITE_COMPONENT));
	VannoEngine::Transform* pTransform = dynamic_cast<VannoEngine::Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));
	VannoEngine::PhysicsBody* pBody = dynamic_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));
	VannoEngine::Animator* pAnimator = dynamic_cast<VannoEngine::Animator*>(GetOwner()->GetComponent(ANIMATOR_COMPONENT));

	VannoEngine::ConfigurationManager* pConfigManager = VannoEngine::ConfigurationManager::GetInstance();

	HandleCollisions(pTransform, pBody);

	if (pInputManager->IsKeyTriggered(ACTION_DEBUG)) {
		pConfigManager->ToggleBool("/debugMode");
		LOG_CORE_DEBUG("Debug Mode is now: {0}", pConfigManager->GetBool("/debugMode"));
	}

	VannoEngine::Collision collision = pBody->GetCollision();
	switch (mCurrentState) {
	case State::Stand:
		pTransform->SetSpeed(0.0f, 0.0f);
		pAnimator->Play("idle");
		if (!collision.CollisionDetected(VannoEngine::Direction::BOTTOM)) {
			mCurrentState = State::Jump;
		}
		if (pInputManager->IsKeyPressed(ACTION_JUMP)) {
			pTransform->SetSpeedY(cJumpSpeed);
			mCurrentState = State::Jump;
		} else if (pInputManager->IsKeyPressed(ACTION_MOVE_LEFT) != pInputManager->IsKeyPressed(ACTION_MOVE_RIGHT)) {
			mCurrentState = State::Walk;
		}
		break;
	case State::Walk:
		pAnimator->Play("walk");
		if (pInputManager->IsKeyPressed(ACTION_MOVE_LEFT) == pInputManager->IsKeyPressed(ACTION_MOVE_RIGHT)) {
			mCurrentState = State::Stand;
			pTransform->SetSpeed(0.0f, 0.0f);
		}
		else if (pInputManager->IsKeyPressed(ACTION_MOVE_LEFT)) {
			pTransform->SetSpeedX(-cWalkSpeed);
			pSprite->SetFlipHorizontal(true);
		}
		else if (pInputManager->IsKeyPressed(ACTION_MOVE_RIGHT)) {
			pTransform->SetSpeedX(cWalkSpeed);
			pSprite->SetFlipHorizontal(false);
		}

		if (pInputManager->IsKeyPressed(ACTION_JUMP)) {
			pTransform->SetSpeedY(cJumpSpeed);
			mCurrentState = State::Jump;
		}
		else if (!collision.CollisionDetected(VannoEngine::Direction::BOTTOM)) {
			mCurrentState = State::Jump;
		}
		break;
	case State::Jump:
		if (pTransform->GetSpeed().y > 0) {
			pAnimator->Play("jump");
		}
		else {
			pAnimator->Play("falling");
		}
		if (collision.CollisionDetected(VannoEngine::Direction::BOTTOM)) {
			mCurrentState = State::Stand;
		}
		/*if (pInputManager->IsKeyPressed(ACTION_MOVE_LEFT) == pInputManager->IsKeyPressed(ACTION_MOVE_RIGHT)) {
			mCurrentState = State::Stand;
			pTransform->SetSpeed(0.0f, 0.0f);
		}
		else if (pInputManager->IsKeyPressed(ACTION_MOVE_LEFT)) {
			pTransform->SetSpeedX(-cWalkSpeed);
			pSprite->SetFlipHorizontal(true);
		}
		else if (pInputManager->IsKeyPressed(ACTION_MOVE_RIGHT)) {
			pTransform->SetSpeedX(cWalkSpeed);
			pSprite->SetFlipHorizontal(false);
		}

		if (pInputManager->IsKeyPressed(ACTION_JUMP)) {
			pTransform->SetSpeedY(cJumpSpeed);
			mCurrentState = State::Jump;
		}
		else if (!collision.CollisionDetected(VannoEngine::Direction::BOTTOM)) {
			mCurrentState = State::Jump;
		}

		if (!pInputManager->IsKeyPressed(ACTION_JUMP) && pTransform->GetSpeed().y > 0.0f) {
			pTransform->SetSpeedY(std::min(pTransform->GetSpeed().y, cMinJumpSpeed));
		}*/
		break;
	}

	VannoEngine::LevelManager* pLevelManager = VannoEngine::LevelManager::GetInstance();
	VannoEngine::Camera* pCamera = pLevelManager->GetCamera();
	VannoEngine::Level* pLevel = pLevelManager->GetCurrentLevel();
	float levelWidth = pLevel->GetWidth();
	float levelHeight = pLevel->GetHeight();
	float hW = pCamera->GetScreenWidth() / 2.0f;
	float hH = pCamera->GetScreenHeight() / 2.0f;
	
	glm::vec2 cameraPosition = pTransform->GetPosition();
	if (cameraPosition.x - hW < 0.0f) {
		cameraPosition.x = hW;
	} else if (cameraPosition.x + hW > levelWidth) {
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