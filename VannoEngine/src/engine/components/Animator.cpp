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

#include "engine/systems/graphics/GraphicsManager.h"
#include "engine/systems/graphics/Colors.h"

#include "engine/systems/ConfigurationManager.h"

#include "engine/systems/objects/GameObject.h"
#include "engine/components/Transform.h"
#include "engine/components/PhysicsBody.h"

#include "engine/core/Log.h"

#include <glm/vec2.hpp>

#include <sstream>

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

				if (animationData.HasMember("aabb") && animationData["aabb"].IsObject()) {
					const rapidjson::Value& aabbData = animationData["aabb"];

					pAnimation->aabbDefined = true;

					if (aabbData.HasMember("offsetX") && aabbData["offsetX"].IsNumber()) {
						pAnimation->aabbOffsetX = aabbData["offsetX"].GetFloat();
					}

					if (aabbData.HasMember("offsetY") && aabbData["offsetY"].IsNumber()) {
						pAnimation->aabbOffsetY = aabbData["offsetY"].GetFloat();
					}

					if (aabbData.HasMember("width") && aabbData["width"].IsNumber()) {
						pAnimation->aabbWidth = aabbData["width"].GetFloat();
					}

					if (aabbData.HasMember("height") && aabbData["height"].IsNumber()) {
						pAnimation->aabbHeight = aabbData["height"].GetFloat();
					}
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

			//LOG_CORE_DEBUG("Animation: {}, Frame: {}, Loop: {}", mpCurrentAnimation->name, mpCurrentAnimation->frameOffset + mFrameIndex, mpCurrentAnimation->loop);
			if (mElapsedFrameTime >= mpCurrentAnimation->frameDuration) {
				if(mFrameIndex < mpCurrentAnimation->frameCount-1 || mpCurrentAnimation->loop) {
					mFrameIndex = (mFrameIndex + 1) % mpCurrentAnimation->frameCount;
					mElapsedFrameTime = 0.0f;
				}
			}
		}
	}

	void Animator::Draw() {
		
		if (ConfigurationManager::GetInstance()->GetBool("/debugMode")) {
			GraphicsManager* pGraphicsManager = GraphicsManager::GetInstance();
			Transform* pTransform = dynamic_cast<Transform*>(GetOwner()->GetComponent(TRANSFORM_COMPONENT));

			std::stringstream ss;
			ss << (mpCurrentAnimation->frameOffset + mFrameIndex);

			glm::vec2 position = pTransform->GetPosition();
			pGraphicsManager->RenderText(ss.str(), position.x+16.0f, position.y - 5.0f, 0.5f, RED);
		}
		
	}

	void Animator::Play(std::string animationName) {
		if(!mpCurrentAnimation || mpCurrentAnimation->name != animationName) {
			if (HasAnimation(animationName)) {
				mpCurrentAnimation = mAnimations[animationName];
				mFrameIndex = 0;
				mElapsedFrameTime = 0.0f;

				if (mpCurrentAnimation->aabbDefined && GetOwner()->HasComponent(PHYSICSBODY_COMPONENT)) {
					PhysicsBody* pBody = dynamic_cast<PhysicsBody*>(GetOwner()->GetComponent(PHYSICSBODY_COMPONENT));

					pBody->SetAabbDimensions(mpCurrentAnimation->aabbWidth, mpCurrentAnimation->aabbHeight);
					pBody->SetAabbOffset(mpCurrentAnimation->aabbOffsetX, mpCurrentAnimation->aabbOffsetY);
				}
			}
			else {
				LOG_CORE_WARN("Unknown animation '{}'", animationName);
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
