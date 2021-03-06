/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		HealthTracker.cpp
Purpose:		Simple component for displaying and tracking health
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-28
*************************************************************************/

#include "HealthTracker.h"

#include "engine/systems/events/DamageEvent.h"
#include "DeathEvent.h"
#include "InvulnerableEvent.h"

#include "engine/systems/graphics/GraphicsManager.h"

#include "engine/systems/events/EventManager.h"

#include "engine/components/PhysicsBody.h"
#include "engine/components/Transform.h"

#include "engine/systems/levels/ObjectMapLayer.h"

#include "engine/core/Log.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/vector_angle.hpp>

HealthTracker::HealthTracker(VannoEngine::GameObject* owner) :
	GameComponent(owner),
	mpGraphicsManager{ VannoEngine::GraphicsManager::GetInstance() },
	mMaxHealth{ 0.0f },
	mCurrentHealth{ 0.0f },
	mVisible{ true },
	mInvulnerable{ false },
	mDamageCooldown{ 0.0 }
{
	VannoEngine::EventManager::GetInstance()->Subscribe(EVT_DAMAGE, this);
}

HealthTracker::~HealthTracker() {
	VannoEngine::EventManager::GetInstance()->Unsubscribe(EVT_DAMAGE, this);
}


void HealthTracker::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
	if (pData->HasMember("maxHealth") && (*pData)["maxHealth"].IsNumber()) {
		mMaxHealth = (*pData)["maxHealth"].GetFloat();
		mCurrentHealth = mMaxHealth;
	}
}

void HealthTracker::Update(double deltaTime) {
	if (mCurrentHealth < 0.0) {
		mCurrentHealth = 0.0;
	}
	if (mDamageCooldown > 0.0) {
		mDamageCooldown -= deltaTime;
	}
	else if (mDamageCooldown < 0.0) {
		mDamageCooldown = 0.0;
	}
}

void HealthTracker::Draw() {
	if(mCurrentHealth < mMaxHealth && mVisible) {
		VannoEngine::PhysicsBody* pBody = dynamic_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));

		glm::vec2 pos = pBody->GetAabbCenter();
		pos.y += pBody->GetAabbHalfDimensions().y + 10.0f;

		float percentage = mCurrentHealth / mMaxHealth;
		float width = 24.0f;
		mpGraphicsManager->RenderSquare(pos, width, 5.0f, glm::vec4(0.0f, 0.3f, 0.0f, 1.0f), true);
		if(mCurrentHealth > 0.0f) {
			pos.x -= (width - (width * percentage)) / 2.0f;
			mpGraphicsManager->RenderSquare(pos, width * percentage, 5.0f, glm::vec4(0.0f, 0.5f, 0.0f, 1.0f), true);
		}
	}
}

void HealthTracker::HandleLocalEvent(std::string eventName, VannoEngine::Event* event) {
	if (event->GetName() == EVT_DAMAGE) {
		VannoEngine::DamageEvent* pEvent = dynamic_cast<VannoEngine::DamageEvent*>(event);

		if(mCurrentHealth > 0.0f && !mInvulnerable && mDamageCooldown == 0.0) {
			mCurrentHealth -= pEvent->GetAmount();
			mDamageCooldown = 0.5;
			LOG_DEBUG("{} took {} damage from {}", GetOwner()->GetName(), pEvent->GetAmount(), (pEvent->GetSource()?pEvent->GetSource()->GetName():"the level"));

			if (mCurrentHealth <= 0.0f) {
				VannoEngine::EventManager::GetInstance()->Direct(GetOwner(), new DeathEvent(GetOwner()));
				mVisible = false;
			}
		}
		else if (mInvulnerable) {
			LOG_DEBUG("{} is invulnerable", GetOwner()->GetName());
		}
		else if (mDamageCooldown != 0.0) {
			LOG_DEBUG("{} damage cooldown is {}", GetOwner()->GetName(), mDamageCooldown);
		}
	}
	else if (event->GetName() == EVT_INVULNERABLE) {
		InvulnerableEvent* pEvent = dynamic_cast<InvulnerableEvent*>(event);

		mInvulnerable = pEvent->GetState();
	}
}

void HealthTracker::HandleEvent(std::string eventName, VannoEngine::Event* event) {
	if (event->GetName() == EVT_DAMAGE) {
		VannoEngine::DamageEvent* pEvent = dynamic_cast<VannoEngine::DamageEvent*>(event);
		if(pEvent->GetTarget() == GetOwner() && mCurrentHealth > 0.0f && !mInvulnerable) {
			if (pEvent->GetAmount() > 0.0f) {
				if (mDamageCooldown <= 0.0) {
					mCurrentHealth -= pEvent->GetAmount();
					mDamageCooldown = 1.0;
					LOG_DEBUG("{} took {} damage from level", pEvent->GetTarget()->GetName(), pEvent->GetAmount());
					if (mCurrentHealth <= 0.0f) {
						VannoEngine::EventManager::GetInstance()->Direct(GetOwner(), new DeathEvent(GetOwner()));
						mVisible = false;
					}
				}
			}
		}
	}
}

