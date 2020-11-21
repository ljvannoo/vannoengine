#include "Aabb.h"

namespace VannoEngine {
	AABB::AABB() :
		halfWidth(0.0f),
		halfHeight(0.0f),
		margin(5.0f)
	{}

	AABB::AABB(float width, float height) :
		halfWidth(width / 2.0f),
		halfHeight(height / 2.0f),
		margin(5.0f)
	{}

	/*
	bool AABB::Collides(AABB const& other) {
		if (fabsf(center.x - other.center.x) > halfWidth + other.halfWidth) {
			return false;
		}
		if (fabsf(center.y - other.center.y) > halfHeight + other.halfHeight) {
			return false;
		}

		return true;
	}
	*/
}