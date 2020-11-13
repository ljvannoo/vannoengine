#include "AudioManager.h"

#include "engine/core/Log.h"

#include <fmod.hpp>
#include <fmod_errors.h>



namespace VannoEngine {
	AudioManager* AudioManager::mpInstance = nullptr;

	AudioManager* AudioManager::GetInstance() {
		if (!mpInstance) {
			mpInstance = new AudioManager;
		}

		return mpInstance;
	}

	AudioManager::AudioManager() :
		mpSystem{nullptr}
	{ }

	AudioManager::~AudioManager() {

	}

	void AudioManager::Init() {
		FMOD_RESULT result;

		result = FMOD::Studio::System::create(&mpSystem);
		if (result != FMOD_OK) {
			LOG_CRITICAL("FMOD error! ({0}) {1}", result, FMOD_ErrorString(result));
		}

		result = mpSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
		if (result != FMOD_OK) {
			LOG_CRITICAL("FMOD error! ({0}) {1}", result, FMOD_ErrorString(result));
		}
	}
}