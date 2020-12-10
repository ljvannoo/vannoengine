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