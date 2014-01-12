#pragma once
#ifndef OPENALSYSTEM_H
#define OPENALSYSTEM_H

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include "glm/glm.hpp"
#include "GLTransform.h"
#include "IFactory.h"
#include "ISystem.h"
#include <memory>
#include <vector>
#include <map>
#include <string>
#include "resources/ALBuffer.h"
#include "resources/SoundFile.h"
#include "components/ALSound.h"
#include "Sigma.h"

namespace Sigma {
	class OpenALSystem
		: public Sigma::IFactory, public ISystem<IComponent> {
		friend class ALSound;
		friend class FactorySystem;
	public:
		DLL_EXPORT OpenALSystem();
		DLL_EXPORT virtual ~OpenALSystem();

		/**
		 * \brief Starts the OpenAL audio system.
		 *
		 * Starts the OpenAL system and sets up an audio context.
		 * \return false on failure, true otherwise.
		 */
		DLL_EXPORT bool Start();

		/**
		 * \brief Update listener position and handle queuing.
		 *
		 * Updates listener position and handles any queuing and/or streaming operations.
		 * \return true if any audio processing was performed
		 */
		DLL_EXPORT bool Update();

		DLL_EXPORT IComponent* CreateALSource(const id_t, const std::vector<Property> &);

		/**
		 * \brief Create an empty SoundFile resource.
		 *
		 * \return long index to SoundFile
		 */
		DLL_EXPORT long CreateSoundFile();

		/**
		 * \brief Create an empty SoundFile resource with a name.
		 *
		 * \return long index to SoundFile
		 */
		DLL_EXPORT long CreateSoundFile(std::string);

		/**
		 * \brief Create a SoundFile resource and load from a file.
		 *
		 * \return long index to SoundFile
		 */
		DLL_EXPORT long LoadSoundFile(std::string);

		/**
		 * \brief Get the SoundFile resource at an index.
		 *
		 * \return std::weak_ptr to SoundFile
		 */
		std::weak_ptr<resource::SoundFile> GetSoundFile(long i) {
			std::weak_ptr<resource::SoundFile> p;
			if (audiofiles.find(i) != audiofiles.end()) {
				p = audiofiles[i];
			}
			return p;
		}

		DLL_EXPORT void StopAll();
		DLL_EXPORT void UpdateTransform(GLTransform &t);
		DLL_EXPORT void UpdateTransform(glm::vec3 pos, glm::vec3 forward, glm::vec3 up);

		DLL_EXPORT void test();
	private:
		std::map<std::string,FactoryFunction> getFactoryFunctions();
		int AllocateBuffer();
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
