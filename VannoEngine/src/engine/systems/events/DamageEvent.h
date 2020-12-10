#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		DamageEvent.h
Purpose:		Signals damage has occurred
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-28
*************************************************************************/
#include "engine/systems/events/Event.h"

#define EVT_DAMAGE "evt_damage"

namespace VannoEngine {
	class GameObject;

	class DamageEvent : public VannoEngine::Event
	{
	public: // Methods
		DamageEvent(VannoEngine::GameObject* pSource, VannoEngine::GameObject* pTarget, float amount) :
			Event(EVT_DAMAGE),
			mpSource{ pSource },
			mpTarget{ pTarget },
			mAmount{ amount }
		{ }
		~DamageEvent() {}

		VannoEngine::GameObject* GetSource() { return mpSource; }
		VannoEngine::GameObject* GetTarget() { return mpTarget; }
		float GetAmount() { return mAmount; }

	private: // Variables
		VannoEngine::GameObject* mpSource;
		VannoEngine::GameObject* mpTarget;
		float mAmount;
	};
}