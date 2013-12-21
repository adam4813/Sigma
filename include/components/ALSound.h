#pragma once

#include "ISound.h"
#include "glm/glm.hpp"
#include <AL/al.h>

namespace Sigma {

	class ALSound : public ISound {
	public:
		SET_COMPONENT_TYPENAME("ALSound");

		ALSound(int entityID) : ISound(entityID), sourceid(0) { }
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
	private:
		ALuint sourceid;
	};
} // namespace Sigma
