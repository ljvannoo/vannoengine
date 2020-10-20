#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS
#include <iostream>

extern VannoEngine::Game* VannoEngine::CreateGame();

int main(int argc, char** argv) {
	std::cout << "Welcome to VannoEngine v0.0.1!" << std::endl;
	auto game = VannoEngine::CreateGame();
	game->Run();
	delete game;
	return 0;
}
#endif