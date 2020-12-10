/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		PowerUp.cpp
Purpose:		Used to define behavior of powerups on the level
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-24
*************************************************************************/

#include "PowerUp.h"

#include "engine/systems/objects/GameObject.h"

#include "engine/components/Transform.h"

PowerUp::PowerUp(VannoEngine::GameObject* owner) :
	GameComponent(owner)
{ }

PowerUp::~PowerUp() {

}

void PowerUp::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {

}

void PowerUp::Update(double deltaTime) {
	VannoEngine::Transform* pTransform = dynamic_cast<VannoEngine::Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));

	if (pTransform->GetSpeed().y == 0.0f) {
		pTransform->SetSpeedX(0.0f);
	}
}