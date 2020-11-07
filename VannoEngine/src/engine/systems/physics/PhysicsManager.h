#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		PhysicsManager.h
Purpose:		Handles physics calculations
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-02
*************************************************************************/
#include "engine/systems/physics/Aabb.h"

namespace VannoEngine {
	struct Collision;

	class PhysicsManager
	{
	public:
		static PhysicsManager* GetInstance();
		~PhysicsManager();

		float GetGravity() { return mGravity; }
		void FlipGravity() { mGravity *= -1.0f;  }
		float GetMaxFallingSpeed() { return 20.0f; }

		Collision const& CollidesWithMap(AABB const& aabb);
	private:
		PhysicsManager();

	private:
		static PhysicsManager* mpInstance;

		float mGravity = -50.0f;
	};
}
