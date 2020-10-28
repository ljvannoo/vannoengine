#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		GLTexture.h
Purpose:		Implements a struct for storing a basic texture
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-10
*************************************************************************/
#include <GL/glew.h>

namespace VannoEngine {
	struct GLTexture {
		GLuint id;

		int width;
		int height;

		unsigned char* pData;
	};
}