
#include "components/ALSound.h"

namespace Sigma {
	void ALSound::Generate() {
		alGenSources(1, &this->sourceid);
	}
	void ALSound::Destroy() {
		if(this->sourceid != 0) {
			alDeleteSources(1, &this->sourceid);
			this->sourceid = 0;
		}
	}

	void ALSound::Play(PLAYBACK mode){}
	void ALSound::Pause(){}
	void ALSound::Stop(){}
	void ALSound::PlayMode(ORDERING o, PLAYBACK mode){}

	void ALSound::Gain(float mul){}

	void ALSound::Position(float x, float y, float z){}
	void ALSound::Position(glm::vec3 v){}
	void ALSound::Velocity(float x, float y, float z){}
	void ALSound::Velocity(glm::vec3 v){}
} // Namespace Sigma
