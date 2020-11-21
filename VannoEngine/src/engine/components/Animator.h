#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Animator.h
Purpose:		Handles sprite animation
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-03
*************************************************************************/
#define ANIMATOR_COMPONENT "animator"

#include "engine/components/GameComponent.h"

#include <string>
#include <unordered_map>
namespace VannoEngine {
	class Animation {
	public:
		std::string name;
		int frameOffset;
		int frameCount;
		float frameDuration;
		bool loop;
		bool aabbDefined;
		float aabbOffsetX, aabbOffsetY;
		float aabbWidth, aabbHeight;
	public:
		Animation() :
			name{ "" },
			frameOffset{ 0 },
			frameCount{ 0 },
			frameDuration{ 0.1f },
			loop{ true },
			aabbDefined{ false },
			aabbOffsetX{ 0.0f },
			aabbOffsetY{ 0.0f },
			aabbWidth{ 0.0f },
			aabbHeight{ 0.0f }
		{}
		~Animation() {}
	};

	class Animator : public GameComponent
	{
	public: // Variables

	public: // Methods
		Animator(GameObject* owner);
		~Animator() override;

		std::string GetType() override {
			return ANIMATOR_COMPONENT;
		}

		void LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) override;

		void Update(double deltaTime) override;
		virtual void Draw() override;

		void Play(std::string animationName);
		std::string GetCurrentAnimation();
		int GetFrameIndex();

	private: // Methods
		bool HasAnimation(std::string name);
		void AddAnimation(Animation* animation);

	private: // Variables
		std::unordered_map <std::string, Animation*> mAnimations;
		Animation* mpCurrentAnimation;

		double mElapsedFrameTime;
		int mFrameIndex;

	};
}
