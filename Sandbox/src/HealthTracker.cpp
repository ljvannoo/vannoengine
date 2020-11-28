#include "HealthTracker.h"

#include "DamageEvent.h"

#include "engine/systems/graphics/GraphicsManager.h"

#include "engine/systems/events/EventManager.h"

#include "engine/components/PhysicsBody.h"
#include "engine/components/Transform.h"

#include "engine/core/Log.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/vector_angle.hpp>

HealthTracker::HealthTracker(VannoEngine::GameObject* owner) :
	GameComponent(owner),
	mpGraphicsManager{ VannoEngine::GraphicsManager::GetInstance() },
	mMaxHealth{ 0.0f },
	mCurrentHealth{ 0.0f }
{
	
}

HealthTracker::~HealthTracker() {
	
}


void HealthTracker::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
	if (pData->HasMember("maxHealth") && (*pData)["maxHealth"].IsNumber()) {
		mMaxHealth = (*pData)["maxHealth"].GetFloat();
		mCurrentHealth = mMaxHealth;
	}
}

void HealthTracker::Update(double deltaTime) {
	if (mCurrentHealth < 0.0f) {
		mCurrentHealth == 0.0f;
	}
}

void HealthTracker::Draw() {
	if(mCurrentHealth < mMaxHealth) {
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
		DamageEvent* pEvent = dynamic_cast<DamageEvent*>(event);

		if(mCurrentHealth > 0.0f) {
			mCurrentHealth -= pEvent->GetAmount();
		}
		LOG_DEBUG("{} took {} damage from {}", GetOwner()->GetName(), pEvent->GetAmount(), pEvent->GetSource()->GetName());
	}
}
