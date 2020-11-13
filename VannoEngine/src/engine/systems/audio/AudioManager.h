#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		AudioManager.h
Purpose:		Controls audio systems in the game
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-7
*************************************************************************/

#include <fmod_studio.hpp>

namespace VannoEngine {
	class AudioManager
	{
	public:
		static AudioManager* GetInstance();
		~AudioManager();

		void Init();
	private:
		AudioManager();

	private:
		static AudioManager* mpInstance;

		FMOD::Studio::System* mpSystem;
	};
}
