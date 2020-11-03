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

#include "engine/systems/objects/GameObject.h"
#include "engine/systems/events/EventManager.h"
#include "engine/systems/FramerateController.h"
#include "engine/systems/InputManager.h"
#include "engine/systems/physics/PhysicsManager.h"

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

	VannoEngine::Sprite* pSprite = static_cast<VannoEngine::Sprite*>(GetOwner()->GetComponent(SPRITE_COMPONENT));
	VannoEngine::Transform* pTransform = static_cast<VannoEngine::Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));
	VannoEngine::PhysicsBody* pBody = static_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));
	VannoEngine::Animator* pAnimator = static_cast<VannoEngine::Animator*>(GetOwner()->GetComponent(ANIMATOR_COMPONENT));

	switch (mCurrentState) {
	case State::Stand:
		pTransform->SetSpeed(0.0f, 0.0f);
		pAnimator->Play("idle");
		if (!pBody->IsOnGround()) {
			mCurrentState = State::Jump;
		}
		if (pInputManager->IsKeyPressed(VannoEngine::Action::JUMP)) {
			pTransform->SetSpeedY(cJumpSpeed);
			mCurrentState = State::Jump;
		} else if (pInputManager->IsKeyPressed(VannoEngine::Action::MOVE_LEFT) != pInputManager->IsKeyPressed(VannoEngine::Action::MOVE_RIGHT)) {
			mCurrentState = State::Walk;
		}
		break;
	case State::Walk:
		pAnimator->Play("walk");
		if (pInputManager->IsKeyPressed(VannoEngine::Action::MOVE_LEFT) == pInputManager->IsKeyPressed(VannoEngine::Action::MOVE_RIGHT)) {
			mCurrentState = State::Stand;
			pTransform->SetSpeed(0.0f, 0.0f);
		}
		else if (pInputManager->IsKeyPressed(VannoEngine::Action::MOVE_LEFT)) {
			pTransform->SetSpeedX(-cWalkSpeed);
			pSprite->SetFlipHorizontal(true);
		}
		else if (pInputManager->IsKeyPressed(VannoEngine::Action::MOVE_RIGHT)) {
			pTransform->SetSpeedX(cWalkSpeed);
			pSprite->SetFlipHorizontal(false);
		}

		if (pInputManager->IsKeyPressed(VannoEngine::Action::JUMP)) {
			pTransform->SetSpeedY(cJumpSpeed);
			mCurrentState = State::Jump;
		}
		else if (!pBody->IsOnGround()) {
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
		if (pBody->IsOnGround()) {
			mCurrentState = State::Stand;
		}
		/*if (pInputManager->IsKeyPressed(VannoEngine::Action::MOVE_LEFT) == pInputManager->IsKeyPressed(VannoEngine::Action::MOVE_RIGHT)) {
			mCurrentState = State::Stand;
			pTransform->SetSpeed(0.0f, 0.0f);
		}
		else if (pInputManager->IsKeyPressed(VannoEngine::Action::MOVE_LEFT)) {
			pTransform->SetSpeedX(-cWalkSpeed);
			pSprite->SetFlipHorizontal(true);
		}
		else if (pInputManager->IsKeyPressed(VannoEngine::Action::MOVE_RIGHT)) {
			pTransform->SetSpeedX(cWalkSpeed);
			pSprite->SetFlipHorizontal(false);
		}

		if (pInputManager->IsKeyPressed(VannoEngine::Action::JUMP)) {
			pTransform->SetSpeedY(cJumpSpeed);
			mCurrentState = State::Jump;
		}
		else if (!pBody->IsOnGround()) {
			mCurrentState = State::Jump;
		}

		if (!pInputManager->IsKeyPressed(VannoEngine::Action::JUMP) && pTransform->GetSpeed().y > 0.0f) {
			pTransform->SetSpeedY(std::min(pTransform->GetSpeed().y, cMinJumpSpeed));
		}*/
		break;
	}
}
