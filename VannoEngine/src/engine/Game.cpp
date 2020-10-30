/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		GameManager.cpp
Purpose:		Implementation of the GameManager.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-05
*************************************************************************/
#include "Game.h"
#include "engine/systems/InputManager.h"
#include "engine/systems/FramerateController.h"
#include "engine/systems/TimeManager.h"
#include "engine/systems/EventManager.h"
#include "engine/systems/GameObjectFactory.h"
#include "engine/systems/GraphicsManager.h"
#include "engine/systems/LevelManager.h"

#include "engine/core/GameObject.h"
#include "engine/core/components/Components.h"
#include "engine/core/components/ComponentCreator.h"

#include "engine/Log.h"

#include <SDL_events.h>


namespace VannoEngine {
	Game::Game():
		IsRunning(false),
		mIsInitialized(false),
		mpInputManager(nullptr),
		mpFramerateManager(nullptr),
		mpEventManager(nullptr),
		mpGraphicsManager(nullptr),
		mpObjectFactory(nullptr),
		mpLevelManager(nullptr),
		mpTimeManager(nullptr),
		mpTestSystem(nullptr)
	{}

	Game::~Game() {

	}

	void Game::Init(int windowWidth, int windowHeight, int fpsCap) {
		/* *****INIT INTERNAL MANAGERS ***** */
		mpObjectFactory = GameObjectFactory::GetInstance();
		mpObjectFactory->Init();

		mpObjectFactory->RegisterComponent(SPRITE_COMPONENT, new ComponentCreator<Sprite>());
		mpObjectFactory->RegisterComponent(TRANSFORM_COMPONENT, new ComponentCreator<Transform>());
		mpObjectFactory->RegisterComponent(CONTROLLER_COMPONENT, new ComponentCreator<Controller>());
		mpObjectFactory->RegisterComponent(CAMERA_COMPONENT, new ComponentCreator<Camera>());

		mpGraphicsManager = GraphicsManager::GetInstance();
		mpGraphicsManager->Init(windowWidth, windowHeight);

		mpInputManager = InputManager::GetInstance();
		mpInputManager->Init();

		mpFramerateManager = FramerateController::GetInstance();
		mpFramerateManager->SetMaxFramerate(fpsCap);

		mpEventManager = EventManager::GetInstance();
		mpEventManager->Subscribe("showFps", this);
		mpEventManager->DelayedNotify(1.0, "showFps", "");

		mpLevelManager = LevelManager::GetInstance();

		mpTimeManager = TimeManager::GetInstance();

		mIsInitialized = true;
		IsRunning = true;
	}

	void Game::Run() {
		while (IsRunning) {
			mpFramerateManager->StartFrame();

			SDL_Event e;
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					IsRunning = false;
				}
			}

			mpInputManager->HandleInput();
			Update();
			mpGraphicsManager->Draw();

			mpFramerateManager->EndFrame();
		}
	}

	void Game::Update() {
		mpEventManager->Update();
		mpLevelManager->Update(mpFramerateManager->GetDeltaTime());
	}

	void Game::HandleEvent(std::string eventName, std::string data) {
		// Print the current frame rate to the console once every second
		mpEventManager->DelayedNotify(1.0, "showFps", "");

		LOG_CORE_DEBUG("FPS: {:.2f}  Delta: {:.4f}", mpFramerateManager->GetFPS(), mpFramerateManager->GetDeltaTime());
	}
}