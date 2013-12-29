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

		ALSound(int entityID,OpenALSystem *m) : ISound(entityID), buffercount(0), bufferindex(0), master(m), sourceid(0), stream(false) { }
		virtual ~ALSound() { Destroy(); }

		void Generate();
		void Destroy();

		void Play(PLAYBACK mode = PLAYBACK_NONE);
		void Pause();
		void Stop();
		void PlayMode(ORDERING o = ORDERING_NONE, PLAYBACK mode = PLAYBACK_NONE);

		void Gain(float mul);

		void Position(float x, float y, float z);
		void Position(glm::vec3 v);
		void Velocity(float x, float y, float z);
		void Velocity(glm::vec3 v);

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
