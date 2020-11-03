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
#include "Animator.h"

#include "engine/core/Log.h"

namespace VannoEngine {
	Animator::Animator(GameObject* owner) :
		GameComponent(owner),
		mpCurrentAnimation(nullptr),
		mElapsedFrameTime(0.0f),
		mFrameIndex(0)
	{}

	Animator::~Animator() {
		for (auto pair : mAnimations) {
			delete pair.second;
		}
		mAnimations.clear();
	}

	void Animator::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
		if (pData->HasMember("animations") && (*pData)["animations"].IsArray()) {
			const rapidjson::Value& animations = (*pData)["animations"];
			for (rapidjson::SizeType i = 0; i < animations.Size(); i++) {
				const rapidjson::Value& animationData = animations[i];

				Animation* pAnimation = new Animation;

				if (animationData.HasMember("name") && animationData["name"].IsString()) {
					pAnimation->name = animationData["name"].GetString();
				}

				if (animationData.HasMember("frameOffset") && animationData["frameOffset"].IsInt()) {
					pAnimation->frameOffset = animationData["frameOffset"].GetInt();
				}

				if (animationData.HasMember("frameCount") && animationData["frameCount"].IsInt()) {
					pAnimation->frameCount = animationData["frameCount"].GetInt();
				}

				if (animationData.HasMember("frameDuration") && animationData["frameDuration"].IsNumber()) {
					pAnimation->frameDuration = animationData["frameDuration"].GetFloat();
				}

				if (animationData.HasMember("loop") && animationData["loop"].IsBool()) {
					pAnimation->loop = animationData["loop"].GetBool();
				}

				AddAnimation(pAnimation);
				if (!mpCurrentAnimation) {
					Play(pAnimation->name);
				}
			}
		}

		if (pData->HasMember("defaultAnimation") && (*pData)["defaultAnimation"].IsString()) {
			Play((*pData)["defaultAnimation"].GetString());
		}
	}

	void Animator::Update(double deltaTime) {
		if(mpCurrentAnimation) {
			mElapsedFrameTime += deltaTime;

			if (mElapsedFrameTime >= mpCurrentAnimation->frameDuration) {
				if(mFrameIndex < mpCurrentAnimation->frameCount || mpCurrentAnimation->loop) {
					mFrameIndex = (mFrameIndex + 1) % mpCurrentAnimation->frameCount;
					mElapsedFrameTime = 0.0f;
				}
			}
		}
	}

	void Animator::Play(std::string animationName) {
		if(!mpCurrentAnimation || mpCurrentAnimation->name != animationName) {
			if (HasAnimation(animationName)) {
				mpCurrentAnimation = mAnimations[animationName];
				mFrameIndex = 0;
				mElapsedFrameTime = 0.0f;
			}
		}
	}

	int Animator::GetFrameIndex() {
		if (mpCurrentAnimation) {
			return mpCurrentAnimation->frameOffset + mFrameIndex;
		}
		return 0;
	}

	std::string Animator::GetCurrentAnimation() {
		if (mpCurrentAnimation) {
			return mpCurrentAnimation->name;
		}
		return "";
	}

	bool Animator::HasAnimation(std::string name) {
		if (mAnimations.find(name) != mAnimations.end()) {
			return true;
		}
		return false;
	}

	void Animator::AddAnimation(Animation* animation) {
		mAnimations[animation->name] = animation;
	}

}
