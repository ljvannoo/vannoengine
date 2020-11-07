#pragma once

#include <glm/vec2.hpp>
namespace VannoEngine {
	class AABB {
	public:
		glm::vec2 center;
		float halfWidth;
		float halfHeight;
		float margin;

		AABB();
		AABB(float x, float y, float width, float height);

		~AABB() {};

		bool Collides(AABB const& other);
	};
}