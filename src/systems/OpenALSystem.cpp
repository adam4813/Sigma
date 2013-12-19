#include "systems/OpenALSystem.h"

namespace Sigma {

	OpenALSystem::OpenALSystem() : device(nullptr), context(nullptr) {
	}

	bool OpenALSystem::Start() {
		device = alcOpenDevice(nullptr);
		if(device == nullptr) {
			return false;
		}
		context = alcCreateContext(device, nullptr);
		if(context == nullptr) {
			return false;
		}
		if(alcMakeContextCurrent(context)) {
			return false;
		}
		return true;
	}

	bool OpenALSystem::Update() {
		return false;
	}

	void OpenALSystem::test() {
		alGenSources(1, &this->testsource);
		testbuff = std::unique_ptr<resource::ALBuffer>(new resource::ALBuffer());
		testbuff->GenerateBuffer();
		testbuff->LoadTestPattern();
		ALuint b = testbuff->GetID();
		alSourceQueueBuffers(this->testsource, 1, &b);
		alSourcePlay(this->testsource);
	}
}