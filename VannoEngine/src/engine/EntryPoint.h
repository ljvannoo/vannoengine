#pragma once
#ifdef ENGINE_RELEASE
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		EntryPoint.h
Purpose:		Defines the main function and handles creation/deletion of the game
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-20
*************************************************************************/


#ifdef ENGINE_PLATFORM_WINDOWS


extern VannoEngine::Game* VannoEngine::CreateGame();

int main(int argc, char** argv) {
	VannoEngine::Log::Init();
	LOG_CORE_WARN("Initialized log");
	LOG_INFO("Welcome to VannoEngine v1.0!");

	auto game = VannoEngine::CreateGame();
	game->Init();
	game->Run();
	delete game;
	return 0;
}
#endif