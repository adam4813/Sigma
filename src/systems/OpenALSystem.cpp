#include "systems/OpenALSystem.h"

namespace Sigma {

	// We need ctor and dstor to be exported to a dll even if they don't do anything
	// this avoids needing to export getFactoryFunctions() which is only used by Sigma
	OpenALSystem::OpenALSystem() : nextindex(1), device(nullptr), context(nullptr) { }
	OpenALSystem::~OpenALSystem() { }

	bool OpenALSystem::Start() {
		const char * alcx;
		this->device = alcOpenDevice(nullptr);
		if(this->device == nullptr) {
			return false;
		}
		this->context = alcCreateContext(device, nullptr);
		if(this->context == nullptr) {
			return false;
		}
		if(!alcMakeContextCurrent(this->context)) {
			return false;
		}
		alcx = alcGetString(this->device, ALC_EXTENSIONS);
		if( alcx != nullptr ) {
			std::cerr << "OpenAL extentions: " << alcx << '\n';
		}
		return true;
	}
	void OpenALSystem::Shutdown() {
		this->StopAll();
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(this->context);
		alcCloseDevice(this->device);
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

	void OpenALSystem::MasterGain(float v) {
		alListenerf(AL_GAIN, v);
	}

	void OpenALSystem::UpdateTransform(GLTransform &t) {
		glm::vec3 pos, forward, up;
		pos = t.GetPosition();
		forward = t.GetForward();
		up = t.GetUp();
		alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
		float fo[6] = {forward.x, forward.y, forward.z, up.x, up.y, up.z };
		alListenerfv(AL_ORIENTATION, fo);
	}

	void OpenALSystem::UpdateTransform(glm::vec3 pos, glm::vec3 forward, glm::vec3 up) {
		alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
		float fo[6] = {forward.x, forward.y, forward.z, up.x, up.y, up.z };
		alListenerfv(AL_ORIENTATION, fo);
	}

	IComponent* OpenALSystem::CreateALSource(const id_t entityID, const std::vector<Property> &properties) {
		ALSound * sound = new ALSound(entityID, this);

		sound->Generate();

		std::string soundFilename;

		long index;
		float x, y, z;
		bool loop = false;
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
			else if (p->GetName() == "gain") {
				sound->Gain(p->Get<float>());
			}
			else if (p->GetName() == "rolloff") {
				sound->Rolloff(p->Get<float>());
			}
			else if (p->GetName() == "maxdist") {
				sound->MaxDistance(p->Get<float>());
			}
			else if (p->GetName() == "refdist") {
				sound->ReferenceDistance(p->Get<float>());
			}
			else if (p->GetName() == "loop") {
				sound->PlayMode(ORDERING_NONE, PLAYBACK_LOOP);
				continue;
			}
			else if (p->GetName() == "soundFilename") {
				soundFilename = p->Get<std::string>();
				index = LoadSoundFile(soundFilename);
				sound->AddSound(index);
				loop = false;
				continue;
			}
		}

		sound->Position(x,y,z);

		this->addComponent(entityID, sound);
		return sound;
	}
	std::map<std::string, Sigma::IFactory::FactoryFunction>
			OpenALSystem::getFactoryFunctions() {
		using namespace std::placeholders;

		std::map<std::string, Sigma::IFactory::FactoryFunction> retval;
		retval["ALSound"] = std::bind(&OpenALSystem::CreateALSource,this,_1,_2);

		return retval;
	}

	void OpenALSystem::StopAll() {
		for (auto eitr = this->_Components.begin(); eitr != this->_Components.end(); ++eitr) {
			for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
				ALSound *sound = dynamic_cast<ALSound *>(citr->second.get());
				sound->Stop();
			}
		}
	}
	bool OpenALSystem::Update() {
		for (auto eitr = this->_Components.begin(); eitr != this->_Components.end(); ++eitr) {
			for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
				ALSound *sound = dynamic_cast<ALSound *>(citr->second.get());
				sound->Update();
			}
		}
		return false;
	}
	int OpenALSystem::AllocateBuffer() {
		int x;
		std::unique_ptr<resource::ALBuffer> testbuff(new resource::ALBuffer());
		testbuff->GenerateBuffer();
		x = this->buffers.size();
		this->buffers.push_back(std::move(testbuff));
		return x;
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
