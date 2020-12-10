#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Colors.h
Purpose:		Defines commonly used colors
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-15
*************************************************************************/

#include <glm/vec4.hpp>

namespace VannoEngine {
	static glm::vec4 YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
	static glm::vec4 PURPLE(1.0f, 0.0f, 1.0f, 1.0f);
	static glm::vec4 RED(1.0f, 0.0f, 0.0f, 1.0f);
	static glm::vec4 GREEN(0.0f, 1.0f, 0.0f, 1.0f);
	static glm::vec4 BLUE(0.0f, 0.0f, 1.0f, 1.0f);
}