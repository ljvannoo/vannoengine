#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Tileset.h
Purpose:		Describes a tileset
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-29
*************************************************************************/

#include "engine/systems/graphics/GLTexture.h"

#include <GL/glew.h>
#include <rapidjson/document.h>

namespace VannoEngine {
	class Tileset
	{
	public: // Variables

	public: // Methods
		Tileset();
		~Tileset();

		void LoadData(const rapidjson::Value* pData);
	private: // Methods

	private: // Variables
		GLuint mVboID; // Vertex buffer
		GLuint mVaoID; // Vertex Array Object
		GLuint mIboID; // Index buffer
		GLTexture* mpTexture;

		int mStartIndex;
	};
}
