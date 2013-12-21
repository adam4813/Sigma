#pragma once
#ifndef OPENALSYSTEM_H
#define OPENALSYSTEM_H

#include <AL/al.h>
#include <AL/alc.h>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include "resources/ALBuffer.h"
#include "resources/SoundFile.h"
#include "components/ALSound.h"
#include "IFactory.h"
#include "ISystem.h"

namespace Sigma {
	class OpenALSystem
		: public Sigma::IFactory, public ISystem<IComponent> {
	public:
		OpenALSystem();
		virtual ~OpenALSystem() {}

		/**
		 * \brief Starts the OpenAL audio system.
		 *
		 * Starts the OpenAL system and sets up an audio context.
		 * \return false on failure, true otherwise.
		 */
		bool Start();

		/**
		 * \brief Update listener position and handle queuing.
		 *
		 * Updates listener position and handles any queuing and/or streaming operations.
		 * \return true if any audio processing was performed
		 */
		bool Update();

		IComponent* CreateALSource(const unsigned int, const std::vector<Property> &);
		std::map<std::string,FactoryFunction> getFactoryFunctions();

		long CreateSoundFile();
		long CreateSoundFile(std::string);
		long LoadSoundFile(std::string);
		std::weak_ptr<resource::SoundFile> GetSoundFile(long i) {
			std::weak_ptr<resource::SoundFile> p;
			if (audiofiles.find(i) != audiofiles.end()) {
				p = audiofiles[i];
			}
			return p;
		}

		void test();
	private:
		std::vector<std::unique_ptr<resource::ALBuffer>> buffers;
		ALuint testsource;
		ALuint lbuf;
		std::map<std::string, long> audioindex;
		std::map<long, std::shared_ptr<resource::SoundFile>> audiofiles;
		long nextindex;
		int lbi;
		bool altn;
ALCdevice* device;
		ALCcontext* context;

	}; // class OpenALSystem
} // namespace Sigma

#endif // OPENALSYSTEM_H
