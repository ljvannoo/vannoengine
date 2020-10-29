#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS


extern VannoEngine::Game* VannoEngine::CreateGame();

int main(int argc, char** argv) {
	VannoEngine::Log::Init();
	LOG_CORE_WARN("Initialized log");
	LOG_INFO("Welcome to VannoEngine v0.0.1!");

	auto game = VannoEngine::CreateGame();
	game->Init();
	game->Run();
	delete game;
	return 0;
}
#endif