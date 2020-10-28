#pragma once

#include "core.h"

namespace VannoEngine {
	class ENGINE_API Game
	{
	public:
		Game();
		virtual ~Game();

		virtual void Init();
		void Run();
	};

	Game* CreateGame();
}

