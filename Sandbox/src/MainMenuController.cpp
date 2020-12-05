#include "MainMenuController.h"

#include "engine/components/Animator.h"

#include "engine/systems/InputManager.h"

#include "engine/systems/levels/LevelManager.h"

#include "engine/systems/events/EventManager.h"
#include "EndLevelEvent.h"

#include "Actions.h"

#include "engine/core/Log.h"

MainMenuController::MainMenuController(VannoEngine::GameObject* owner) :
	GameComponent(owner),
	mpInputManager{ VannoEngine::InputManager::GetInstance() },
	mCurrentState{ State::PlayGame },
	mFirstLevelFilename{ std::string() }
{ }

MainMenuController::~MainMenuController()
{ }


void MainMenuController::LoadData(const rapidjson::GenericObject<true, rapidjson::Value>* pData) {
	if (pData->HasMember("firstLevel") && (*pData)["firstLevel"].IsString()) {
		mFirstLevelFilename = (*pData)["firstLevel"].GetString();
	}
}

void MainMenuController::Update(double deltaTime) {
	VannoEngine::GameObject* pObj = GetOwner();
	VannoEngine::GameObject* pControlsScreen = pObj->GetChildObject(4);
	VannoEngine::GameObject* pPlayGameObj = pObj->GetChildObject(1);
	VannoEngine::Animator* pPlayGameAnimator = dynamic_cast<VannoEngine::Animator*>(pPlayGameObj->GetComponent(ANIMATOR_COMPONENT));
	VannoEngine::GameObject* pViewControls = pObj->GetChildObject(2);
	VannoEngine::Animator* pViewControlsAnimator = dynamic_cast<VannoEngine::Animator*>(pViewControls->GetComponent(ANIMATOR_COMPONENT));
	VannoEngine::GameObject* pQuitGame = pObj->GetChildObject(3);
	VannoEngine::Animator* pQuitGameAnimator = dynamic_cast<VannoEngine::Animator*>(pQuitGame->GetComponent(ANIMATOR_COMPONENT));

	pControlsScreen->SetVisible(false);
	switch (mCurrentState) {
	case State::PlayGame:
		pPlayGameAnimator->Play("selected");
		pViewControlsAnimator->Play("unselected");
		pQuitGameAnimator->Play("unselected");

		if (mpInputManager->IsKeyTriggered(ACTION_DOWN) || mpInputManager->IsKeyTriggered(ACTION_MENU_DOWN)) {
			mCurrentState = State::ViewControls;
		}

		if (mpInputManager->IsKeyTriggered(ACTION_MENU_SELECT)) {
			if(!mFirstLevelFilename.empty()) {
				VannoEngine::LevelManager::GetInstance()->LoadLevel(mFirstLevelFilename);
			}
		}
		break;
	case State::ViewControls:
		pPlayGameAnimator->Play("unselected");
		pViewControlsAnimator->Play("selected");
		pQuitGameAnimator->Play("unselected");

		if (mpInputManager->IsKeyTriggered(ACTION_UP) || mpInputManager->IsKeyTriggered(ACTION_MENU_UP)) {
			mCurrentState = State::PlayGame;
		} else if (mpInputManager->IsKeyTriggered(ACTION_DOWN) || mpInputManager->IsKeyTriggered(ACTION_MENU_DOWN)) {
			mCurrentState = State::QuitGame;
		}

		if (mpInputManager->IsKeyTriggered(ACTION_MENU_SELECT)) {
			mCurrentState = State::ShowingControls;
			
		}
		break;
	case State::ShowingControls:
		pControlsScreen->SetVisible(true);
		if (mpInputManager->IsAnyKeyTriggered()) {
			mCurrentState = State::ViewControls;
		}
		break;
	case State::QuitGame:
		pPlayGameAnimator->Play("unselected");
		pViewControlsAnimator->Play("unselected");
		pQuitGameAnimator->Play("selected");

		if (mpInputManager->IsKeyTriggered(ACTION_UP) || mpInputManager->IsKeyTriggered(ACTION_MENU_UP)) {
			mCurrentState = State::ViewControls;
		}

		if (mpInputManager->IsKeyTriggered(ACTION_MENU_SELECT)) {
			VannoEngine::EventManager::GetInstance()->Broadcast(new EndLevelEvent());
		}
		break;
	}
}

void MainMenuController::Draw() {

}

