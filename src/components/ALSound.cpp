
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
	void ALSound::Update() {
		ALint param;
		long sfi;
		int i;
		int buflen;
		int chancount;
		int samplecount;
		ALuint albuf;
		short *buf;
		std::shared_ptr<resource::SoundFile> sfp;
		
		if(playing) {
			alGetSourcei(this->sourceid, AL_BUFFER, &param);
			albuf = master->buffers[this->buffers[this->bufferindex]]->GetID();
			if(param != 0 && albuf != param && playlist.size() > 0) {
				sfi = playlist[playindex];
				sfp = std::shared_ptr<resource::SoundFile>(master->GetSoundFile(sfi));
				if(sfp->isStream()) {
					chancount = sfp->Channels();
					samplecount = sfp->Frequency(); // 1 sec buffers
					buflen = chancount * samplecount;
					buf = new short[buflen];
					if(++this->bufferindex >= this->buffercount) { this->bufferindex = 0; }
					i = codec.FetchBuffer(*sfp, buf, ((chancount == 1) ? resource::PCM_MONO16 : resource::PCM_STEREO16), samplecount);
					alSourceUnqueueBuffers(this->sourceid, 1, &albuf);
					if(i > 0) {
						alBufferData(albuf, ((chancount == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16), buf, buflen * sizeof(short), samplecount);
						alSourceQueueBuffers(this->sourceid, 1, &albuf);
					} else {
						if(playloop == PLAYBACK_LOOPING) {
							this->codec.Rewind(*sfp);
						} else {
							playing = false;
						}
					}
					delete buf;
				}
			}
		}
	}
	void ALSound::Play(PLAYBACK mode) {
		long sfi;
		int i, x;
		int buflen;
		int chancount;
		int samplecount;
		ALuint albuf[ALSOUND_BUFFERS];
		unsigned short *buf;
		std::shared_ptr<resource::SoundFile> sfp;

		if(mode != PLAYBACK_NONE) {
			playloop = mode;
		}
		if(paused && !playing) {
			alSourcePlay(this->sourceid);
			playing = true;
			paused = false;
			return;
		}
		if(playlist.size() > 0) {
			if(playindex > playlist.size()) { playindex = 0; }
			sfi = playlist[playindex];
			sfp = std::shared_ptr<resource::SoundFile>(master->GetSoundFile(sfi));
			if(sfp->isStream()) {
				while(this->buffercount < ALSOUND_BUFFERS) {
					this->buffers[this->buffercount++] = master->AllocateBuffer();
				}
				this->bufferindex = 0;
				chancount = sfp->Channels();
				samplecount = sfp->Frequency(); // 1 sec buffers
				buflen = chancount * samplecount;
				this->codec.Rewind(*sfp);
				buf = new unsigned short[buflen];
				x = 0;
				while(x < this->buffercount && 0 < (i = codec.FetchBuffer(*sfp, buf, ((chancount == 1) ? resource::PCM_MONO16 : resource::PCM_STEREO16), samplecount))) {
					albuf[x] = master->buffers[this->buffers[x]]->GetID();
					alBufferData(albuf[x], ((chancount == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16), buf, buflen * 2, samplecount);
					x++;
				}
				delete buf;
				if(i > 0) {
					alSourceQueueBuffers(this->sourceid, ALSOUND_BUFFERS, albuf);
					alSourcePlay(this->sourceid);
					playing = true;
					paused = false;
				}
				return;
			}
		}
	}
	void ALSound::Pause() {
		alSourcePause(this->sourceid);
		paused = true;
		playing = false;
	}
	void ALSound::Stop() {
		alSourceStop(this->sourceid);
		paused = false;
		playing = false;
	}
	void ALSound::PlayMode(ORDERING o, PLAYBACK mode){}

	void ALSound::Gain(float mul){}

	void ALSound::Position(float x, float y, float z){}
	void ALSound::Position(glm::vec3 v){}
	void ALSound::Velocity(float x, float y, float z){}
	void ALSound::Velocity(glm::vec3 v){}
} // Namespace Sigma
