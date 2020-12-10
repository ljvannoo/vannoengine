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