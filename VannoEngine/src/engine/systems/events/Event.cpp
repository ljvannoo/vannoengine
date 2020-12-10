/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Event.cpp
Purpose:		The main event class, used to send messages between game objects
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-23
*************************************************************************/


#include "Event.h"

namespace VannoEngine {
	Event::Event(std::string name) :
		mName{ name }
	{ }

	Event::~Event() {

	}
}
