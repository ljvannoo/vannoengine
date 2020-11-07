#include "Aabb.h"

namespace VannoEngine {
	AABB::AABB() :
		center(glm::vec2(0.0f, 0.0f)),
		halfWidth(0.0f),
		halfHeight(0.0f),
		margin(5.0f)
	{}

	AABB::AABB(float x, float y, float width, float height) :
		center(glm::vec2(x, y)),
		halfWidth(width / 2.0f),
		halfHeight(height / 2.0f)
	{}

	bool AABB::Collides(AABB const& other) {
		if (fabsf(center.x - other.center.x) > halfWidth + other.halfWidth) {
			return false;
		}
		if (fabsf(center.y - other.center.y) > halfHeight + other.halfHeight) {
			return false;
		}

		return true;
	}
}