#include "PowerUp.h"

#include "engine/systems/objects/GameObject.h"

PowerUp::PowerUp(VannoEngine::GameObject* owner) :
	GameComponent(owner)
{ }

PowerUp::~PowerUp() {

}

void PowerUp::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {

}