#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		MapLayer.h
Purpose:		Provides an interface for map layers
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-29
*************************************************************************/
#include <rapidjson/document.h>
#include <glm/vec2.hpp>

#include <string>
namespace VannoEngine {
	// Forward declarations
	class ShaderProgram;

	class MapLayer
	{
	public: // Variables

	public: // Methods
		MapLayer() : 
			mName(""),
			mPosition(glm::vec2(0.0f, 0.0f)), 
			mDimensions(glm::vec2(0.0f, 0.0f))
		{}

		virtual ~MapLayer() {}

		virtual void UpdatePhysics(double deltaTime) {};
		virtual void Update(double deltaTime) = 0;
		virtual void Draw() = 0;

		virtual std::string GetType() = 0;

		virtual void LoadData(const rapidjson::Value* pData) = 0;

		void SetPosition(float x, float y) { mPosition = glm::vec2(x, y); }
		void SetDimensions(float width, float height) { mDimensions = glm::vec2(width, height); }
		float GetWidth() { return mDimensions.x; }
		float GetHeight() { return mDimensions.y; }
	protected:
		glm::vec2 mPosition;
		glm::vec2 mDimensions;
		std::string mName;

	private: // Methods

	private: // Variables
	};
}
