#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Event.h
Purpose:		The main event class, used to send messages between game objects
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-23
*************************************************************************/

#include <string>

namespace VannoEngine {
	class Event
	{
	public: // Variables

	public: // Methods
		Event(std::string name);
		virtual ~Event();

		std::string GetName() const { return mName; }
	private:
		std::string mName;
	};
}
