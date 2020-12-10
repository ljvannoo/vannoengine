#include "ChestController.h"

#include "engine/core/Log.h"

#include "engine/systems/events/DamageEvent.h"

#include "engine/components/Transform.h"
#include "engine/components/PhysicsBody.h"
#include "engine/components/Animator.h"

#include "engine/systems/objects/GameObjectFactory.h"

ChestController::ChestController(VannoEngine::GameObject* owner) :
	GameComponent(owner),
	mContents{ std::string() },
	mCurrentState{ State::Closed }
{ }

ChestController::~ChestController()
{ }


void ChestController::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
	if (pData->HasMember("contents") && (*pData)["contents"].IsString()) {
		mContents = (*pData)["contents"].GetString();
	}
}

void ChestController::Update(double deltaTime) {
	//VannoEngine::Sprite* pSprite = dynamic_cast<VannoEngine::Sprite*>(GetOwner()->GetComponent(SPRITE_COMPONENT));
	VannoEngine::Transform* pTransform = dynamic_cast<VannoEngine::Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));
	VannoEngine::PhysicsBody* pBody = dynamic_cast<VannoEngine::PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));
	VannoEngine::Animator* pAnimator = dynamic_cast<VannoEngine::Animator*>(GetOwner()->GetComponent(ANIMATOR_COMPONENT));
	switch (mCurrentState) {
	case State::Closed:
		pAnimator->Play("shine");
		break;
	case State::Opening:
		pAnimator->Play("open");

		if(!mContents.empty()) {
			VannoEngine::GameObject* pContents = VannoEngine::GameObjectFactory::GetInstance()->CreateObject(mContents, GetOwner()->GetMapLayer());
			VannoEngine::Transform* pContentsTransform = dynamic_cast<VannoEngine::Transform*>(pContents->GetComponent(TRANSFORM_COMPONENT));
			VannoEngine::PhysicsBody* pContentsBody = dynamic_cast<VannoEngine::PhysicsBody*>(pContents->GetComponent(PHYSICSBODY_COMPONENT));
			if (pContentsTransform) {
				glm::vec2 pos = pTransform->GetPosition();
				pContentsTransform->SetPosition(pos.x, pos.y);
				pContentsTransform->SetSpeed(0.5f, 5.0f);
			}
		}
		mCurrentState = State::Open;
		break;
	case State::Open:
		break;
	}
}

void ChestController::Draw() {

}

void ChestController::HandleLocalEvent(std::string eventName, VannoEngine::Event* event) {
	if (event->GetName() == EVT_DAMAGE && mCurrentState == State::Closed) {
		//VannoEngine::DamageEvent* pEvent = dynamic_cast<VannoEngine::DamageEvent*>(event);

		mCurrentState = State::Opening;
	}
}
