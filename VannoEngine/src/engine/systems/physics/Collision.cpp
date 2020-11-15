#include "Collision.h"

namespace VannoEngine {
	Collision::Collision() :
		mType{CollisionType::NONE, CollisionType::NONE, CollisionType::NONE ,CollisionType::NONE },
		mEdge{0.0f, 0.0f, 0.0f, 0.0f},
		mHasCollided{ false }
	{ }

	Collision::~Collision() {

	}

	void Collision::SetCollision(Direction dir, CollisionType type, float edge) {
		mType[int(dir)] = type;
		mEdge[int(dir)] = edge;
		mHasCollided = true;
	}
}
