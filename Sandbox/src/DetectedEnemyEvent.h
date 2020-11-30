#pragma once
#include "engine/systems/events/Event.h"

#define EVT_DETECTED_ENEMY "evt_detected_enemy"

namespace VannoEngine {
	class GameObject;
}
class DetectedEnemyEvent : public VannoEngine::Event
{
public: // Methods
	DetectedEnemyEvent(VannoEngine::GameObject* mObj, VannoEngine::GameObject* pEnemy) :
		Event(EVT_DETECTED_ENEMY),
		mpObj{ mObj },
		mpEnemy{ pEnemy }
	{ }
	~DetectedEnemyEvent() {}

	VannoEngine::GameObject* GetObj() { return mpObj; }
	VannoEngine::GameObject* GetEnemy() { return mpEnemy; }

private: // Variables
	VannoEngine::GameObject* mpObj;
	VannoEngine::GameObject* mpEnemy;
};
