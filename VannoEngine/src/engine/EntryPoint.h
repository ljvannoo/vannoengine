#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS
#include <iostream>

extern VannoEngine::Game* VannoEngine::CreateGame();

int main(int argc, char** argv) {
	VannoEngine::Log::Init();
	VE_CORE_WARN("Initialized log");
	VE_INFO("Welcome to VannoEngine v0.0.1!");

	auto game = VannoEngine::CreateGame();
	game->Init(60);
	game->Run();
	delete game;
	return 0;
}
#endif