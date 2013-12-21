#include "systems/OpenALSystem.h"
#include <iostream>

namespace Sigma {

	OpenALSystem::OpenALSystem() : nextindex(1), device(nullptr), context(nullptr) {
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
		if(!alcMakeContextCurrent(context)) {
			return false;
		}
		return true;
	}
	long OpenALSystem::CreateSoundFile() {
		std::shared_ptr<resource::SoundFile> sound(new resource::SoundFile);
		long currentindex = nextindex++;
		this->audiofiles[currentindex] = sound;
		return currentindex;
	}
	long OpenALSystem::CreateSoundFile(std::string name) {
		if (audioindex.find(name) == audioindex.end()) {
			std::weak_ptr<resource::SoundFile> sound;
			long i = OpenALSystem::CreateSoundFile();
			this->audioindex[name] = i;
			return i;
		} else {
			return this->audioindex[name];
		}
	}
	long OpenALSystem::LoadSoundFile(std::string filename) {
		if (audioindex.find(filename) == audioindex.end()) {
			std::shared_ptr<resource::SoundFile> sound;
			long i = OpenALSystem::CreateSoundFile();
			sound = this->audiofiles[i];
			sound->LoadFromFile(filename);
			if (sound->isLoaded()) {
				this->audioindex[filename] = i;
				return i;
			} else {
				this->audiofiles.erase(i);
				return 0;
			}
		} else {
			return this->audioindex[filename];
		}
	}

	IComponent* OpenALSystem::CreateALSource(const unsigned int entityID, const std::vector<Property> &properties) {
		ALSound * sound = new ALSound(entityID);

		std::string soundFilename;

		long index;
		float x, y, z;
		x = y = z = 0.0f;

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			const Property*  p = &(*propitr);

			if (p->GetName() == "x") {
				x = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "y") {
				y = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "z") {
				z = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "soundFilename") {
				soundFilename = p->Get<std::string>();
				index = LoadSoundFile(soundFilename);
				sound->AddSound(index);
				continue;
			}
		}

		sound->Position(x,y,z);

		return sound;
	}
	std::map<std::string, Sigma::IFactory::FactoryFunction>
			OpenALSystem::getFactoryFunctions() {
		using namespace std::placeholders;

		std::map<std::string, Sigma::IFactory::FactoryFunction> retval;
		retval["ALSound"] = std::bind(&OpenALSystem::CreateALSource,this,_1,_2);

		return retval;
	}

	bool OpenALSystem::Update() {
		ALint i;
		//this whole function is a hack right now!
		alGetSourcei(this->testsource, AL_BUFFER, &i);
		if(i == 0) { alSourcePlay(this->testsource); }
		if(lbuf != i) {
			ALuint b = buffers[lbi]->GetID();
			if(i != 0 && i != b) { // Test "streaming" data, by swapping between two buffers and updating one of them
				alSourceUnqueueBuffers(this->testsource, 1, &b);
				if(lbi) { buffers[lbi]->SineSynth((this->altn = !this->altn) ? 0.03f : 0.02f); }
				alSourceQueueBuffers(this->testsource, 1, &b);
				lbi = lbi ^ 1;
			}
			lbuf = i;
		}
		std::cerr << i << "," << lbuf << ": ";
		return false;
	}

	void OpenALSystem::test() {
		lbi = 0;
		alGenSources(1, &this->testsource);
		std::unique_ptr<resource::ALBuffer> testbuff(new resource::ALBuffer());
		testbuff->GenerateBuffer();
		testbuff->SineSynth(0.04f);
		ALuint b[1];
		b[0] = testbuff->GetID();
		this->buffers.push_back(std::move(testbuff));
		testbuff = std::unique_ptr<resource::ALBuffer>(new resource::ALBuffer());
		testbuff->GenerateBuffer();
		testbuff->SineSynth(0.04f);
		this->buffers.push_back(std::move(testbuff));
		alSourceQueueBuffers(this->testsource, 1, b);
		alSourcei(this->testsource, AL_LOOPING, AL_FALSE);
		alSourcePlay(this->testsource);

		buffers[1]->SineSynth(0.04f);
		b[0] = buffers[1]->GetID();
		alSourceQueueBuffers(this->testsource, 1, b);
	}
}
