#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		EventHandler.h
Purpose:		An interface for responding to events
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-13
*************************************************************************/



#include <string>
namespace VannoEngine {
	class Message;

	class EventHandler
	{
	public:
		virtual ~EventHandler() {};
		virtual void HandleEvent(std::string eventName, Message* message) = 0;
	};
}
