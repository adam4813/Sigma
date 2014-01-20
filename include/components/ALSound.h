#pragma once

#ifdef __APPLE__
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif

#include "ISound.h"
#include "glm/glm.hpp"
#include "systems/OpenALSystem.h"

#define ALSOUND_BUFFERS 4
namespace Sigma {
	class OpenALSystem;

	class ALSound : public ISound {
		friend class OpenALSystem;
	public:
		SET_COMPONENT_TYPENAME("ALSound");

		DLL_EXPORT ALSound(id_t entityID,OpenALSystem *m);
		DLL_EXPORT virtual ~ALSound();

		DLL_EXPORT void Generate();
		DLL_EXPORT void Destroy();

		DLL_EXPORT void Play(PLAYBACK mode = PLAYBACK_NONE);
		DLL_EXPORT void Pause();
		DLL_EXPORT void Stop();
		DLL_EXPORT void PlayMode(ORDERING o = ORDERING_NONE, PLAYBACK mode = PLAYBACK_NONE);

		DLL_EXPORT void Gain(float mul);

		DLL_EXPORT void Rolloff(float atten);
		DLL_EXPORT void ReferenceDistance(float r);
		DLL_EXPORT void MaxDistance(float r);
		DLL_EXPORT void Pitch(float r);

		DLL_EXPORT void Position(float x, float y, float z);
		DLL_EXPORT void Position(glm::vec3 v);
		DLL_EXPORT void Velocity(float x, float y, float z);
		DLL_EXPORT void Velocity(glm::vec3 v);

		ALuint GetID() { return sourceid; }
	protected:
		void Update();
		ALuint sourceid;
		bool stream;
		int buffers[ALSOUND_BUFFERS];
		int streamend;
		int bufferindex;
		int buffercount;
		resource::Decoder codec;
		OpenALSystem *master;
	};
} // namespace Sigma
