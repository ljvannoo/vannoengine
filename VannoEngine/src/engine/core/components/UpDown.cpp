/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		UpDown.cpp
Purpose:		Simple component for moving an object up and down indefinitely
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-24
*************************************************************************/
#include "UpDown.h"

#include "Transform.h"
#include "engine/core/GameObject.h"

namespace VannoEngine {
	UpDown::UpDown(GameObject* owner) :
		GameComponent(owner),
		mMovingUp(false),
		mMaxDistance(10.0f),
		mDistance(0.0f)
	{ }

	UpDown::~UpDown() {
	}

	void UpDown::Update(double deltaTime) {
		Transform* pTransform = static_cast<Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));

		if (pTransform) {
			float direction = 1.0f;
			if (!mMovingUp) {
				direction = -1.0f;
			}

			float curY = pTransform->GetPosition().y;
			float speed = pTransform->GetSpeed() * deltaTime;
			pTransform->SetPositionY(curY + (speed * direction));
			mDistance += speed;

			if (mDistance > mMaxDistance) {
				mMovingUp = !mMovingUp;
				mDistance = 0.0f;
			}
		}
	}

	void UpDown::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
		if (pData->HasMember("startMovingUp") && (*pData)["startMovingUp"].IsBool()) {
			mMovingUp = (*pData)["startMovingUp"].GetBool();
		}

		if (pData->HasMember("distance") && (*pData)["distance"].IsFloat()) {
			mMaxDistance = (*pData)["distance"].GetFloat();
		}
	}
}