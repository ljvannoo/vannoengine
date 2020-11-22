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
#include "engine/systems/events/EventManager.h"
#include "engine/systems/objects/GameObjectFactory.h"
#include "engine/systems/graphics/GraphicsManager.h"
#include "engine/systems/levels/LevelManager.h"

#include "engine/systems/objects/GameObject.h"
#include "engine/components/Components.h"
#include "engine/components/ComponentCreator.h"

#include "engine/core/Log.h"

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
		mpObjectFactory->RegisterComponent(CAMERA_COMPONENT, new ComponentCreator<Camera>());
		mpObjectFactory->RegisterComponent(PHYSICSBODY_COMPONENT, new ComponentCreator<PhysicsBody>());
		mpObjectFactory->RegisterComponent(ANIMATOR_COMPONENT, new ComponentCreator<Animator>());

		mpGraphicsManager = GraphicsManager::GetInstance();
		mpGraphicsManager->Init(windowWidth, windowHeight);

		mpInputManager = InputManager::GetInstance();
		mpInputManager->Init();

		mpFramerateManager = FramerateController::GetInstance();
		mpFramerateManager->SetMaxFramerate(fpsCap);

		mpEventManager = EventManager::GetInstance();

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
			double deltaTime = mpFramerateManager->GetDeltaTime();

			// Input
			mpInputManager->HandleInput();
			HandleInput();

			// Physics
			mpTimeManager->StartTimer("physics");
			mpLevelManager->UpdatePhysics(deltaTime);
			UpdatePhysics(deltaTime);
			mpTimeManager->StopTimer("physics");

			// Update
			mpTimeManager->StartTimer("update");
			mpEventManager->Update();
			mpLevelManager->Update(deltaTime);
			Update(deltaTime);
			mpTimeManager->StopTimer("update");

			// Draw
			mpTimeManager->StartTimer("draw");
			mpGraphicsManager->StartDraw();
			LevelManager::GetInstance()->Draw();
			Draw();
			mpGraphicsManager->EndDraw();
			mpTimeManager->StopTimer("draw");

			mpFramerateManager->EndFrame();
		}
	}
}