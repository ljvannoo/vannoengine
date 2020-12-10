#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		AABB.h
Purpose:		Defines a basic Axis Aligned Bounding Box
				containers and destroyed.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-17
*************************************************************************/

#include <glm/vec2.hpp>
namespace VannoEngine {
	class AABB {
	public:
		//glm::vec2 center;
		float halfWidth;
		float halfHeight;
		float margin;

		AABB();
		AABB(float width, float height);

		~AABB() {};

		//bool Collides(AABB const& other);
	};
}