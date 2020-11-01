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
#include "Transform.h"
#include "Sprite.h"
#include "engine/systems/objects/GameObject.h"
#include "engine/systems/events/EventManager.h"
#include "engine/systems/FramerateController.h"
#include "engine/systems/InputManager.h"



namespace VannoEngine {
	Controller::~Controller() {
	}

	void Controller::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
		EventManager* pEventManager = EventManager::GetInstance();

		pEventManager->Subscribe(EVT_MOVE, this);
	}

	void Controller::Update(double deltaTime) {
		InputManager* pInputManager = InputManager::GetInstance();

		Sprite* pSprite = static_cast<Sprite*>(GetOwner()->GetComponent(SPRITE_COMPONENT));

		if (pSprite) {

			if (pInputManager->IsKeyPressed(SDL_SCANCODE_A) || pInputManager->IsKeyPressed(SDL_SCANCODE_D)) {
				pSprite->SetFrameOffset(8);
				pSprite->SetAnimationDuration(0.1);
			}
			else {
				pSprite->SetFrameOffset(0);
				pSprite->SetAnimationDuration(0.2);
			}
		}
	}

	void Controller::HandleEvent(std::string eventName, std::string data) {
		if (eventName == EVT_MOVE) {
			GameObject* pObject = GetOwner();
			Transform* pTransform = static_cast<Transform*>(pObject->GetComponent(TRANSFORM_COMPONENT));
			Sprite* pSprite = static_cast<Sprite*>(pObject->GetComponent(SPRITE_COMPONENT));
			if (pTransform) {
				FramerateController* pFramerateController = FramerateController::GetInstance();
				if (data == "up") {
					pTransform->SetPositionY(pTransform->GetPosition().y + pTransform->GetSpeed() * static_cast<float>(pFramerateController->GetDeltaTime()));
				}
				else if (data == "down") {
					pTransform->SetPositionY(pTransform->GetPosition().y - pTransform->GetSpeed() * static_cast<float>(pFramerateController->GetDeltaTime()));
				}
				else if (data == "right") {
					if (pSprite->IsHorizontalFlipped()) {
						pSprite->FlipHorizontal();
					}
					pTransform->SetPositionX(pTransform->GetPosition().x + pTransform->GetSpeed() * static_cast<float>(pFramerateController->GetDeltaTime()));
				}
				else if (data == "left") {
					if (!pSprite->IsHorizontalFlipped()) {
						pSprite->FlipHorizontal();
					}
					pTransform->SetPositionX(pTransform->GetPosition().x - pTransform->GetSpeed() * static_cast<float>(pFramerateController->GetDeltaTime()));
				}
			}
		}
	}
}