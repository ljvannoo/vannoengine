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

#include <string>
namespace VannoEngine {
	// Forward declarations
	class ShaderProgram;

	class MapLayer
	{
	public: // Variables

	public: // Methods
		MapLayer() : mpShaderProgram(nullptr) {}
		virtual ~MapLayer() {}

		virtual void Draw() = 0;

		virtual std::string GetType() = 0;

		virtual void LoadData(const rapidjson::Value* pData) = 0;

		ShaderProgram* GetShaderProgram() { return mpShaderProgram; }

	protected:
		ShaderProgram* mpShaderProgram;

	private: // Methods

	private: // Variables
	};
}
