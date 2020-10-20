#include <VannoEngine.h>

class Sandbox : public VannoEngine::Game {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

VannoEngine::Game* VannoEngine::CreateGame() {
	return new Sandbox();
}