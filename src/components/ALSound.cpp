
#include "components/ALSound.h"

namespace Sigma {
	ALSound::ALSound(id_t entityID,OpenALSystem *m)
		: ISound(entityID), buffercount(0), bufferindex(0), master(m), sourceid(0), stream(false) { }
	ALSound::~ALSound() {
		Destroy();
	}
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
		int bufbytes;
		int chancount;
		int samplecount;
		int samplerate;
		ALuint albuf;
		short *buf;
		std::shared_ptr<resource::SoundFile> sfp;

		if(playing) {
			alGetSourcei(this->sourceid, AL_BUFFERS_PROCESSED, &param);
			albuf = master->buffers[this->buffers[this->bufferindex]]->GetID();
			if(param != 0 && playlist.size() > 0) {
				sfi = playlist[playindex];
				sfp = std::shared_ptr<resource::SoundFile>(master->GetSoundFile(sfi));
				if(stream) {
					chancount = sfp->Channels();
					samplerate = sfp->Frequency();
					samplecount = samplerate * 2; // 2 sec buffers
					buflen = chancount * samplecount;
					bufbytes = buflen * sizeof(short);
					buf = new short[buflen];
					if(++this->bufferindex >= this->buffercount) {
						this->bufferindex = 0;
						ALDEBUG(std::cerr << "ALSound: Buffer-set wrap around\n";)
					}
					i = codec.FetchBuffer(*sfp, buf, ((chancount == 1) ? resource::PCM_MONO16 : resource::PCM_STEREO16), samplecount);
					alSourceUnqueueBuffers(this->sourceid, 1, &albuf);
					if(i > 0) {
						if(i < samplecount) {
							buflen = chancount * i;
							if(playloop == PLAYBACK_LOOP) {
								this->codec.Rewind(*sfp);
								ALDEBUG(std::cerr << "ALSound: Looping stream at sample: " << i << '\n';)
								bufbytes = i;
								i = codec.FetchBuffer(*sfp, buf+bufbytes, ((chancount == 1) ? resource::PCM_MONO16 : resource::PCM_STEREO16), samplecount - i);
								buflen = bufbytes+i;
								buflen *= chancount;
							} else {
								ALDEBUG(std::cerr << "ALSound: Playback done; at sample: " << i << '\n';)
								playing = false;
							}
							bufbytes = buflen * sizeof(short);
							alBufferData(albuf, ((chancount == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16), buf, bufbytes, samplerate);
						}
						else {
							alBufferData(albuf, ((chancount == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16), buf, bufbytes, samplerate);
						}
						alSourceQueueBuffers(this->sourceid, 1, &albuf);
					} else {
						if(playloop == PLAYBACK_LOOP) {
							this->codec.Rewind(*sfp);
						} else {
							playing = false;
						}
					}
					delete buf;
				}
			}
			else if(param == this->buffercount && stream) {
				ALDEBUG(std::cerr << "ALSound: Playback underrun\n";)
				alSourcei(this->sourceid, AL_LOOPING, AL_FALSE);
			}
		}
	}
	void ALSound::Play(PLAYBACK mode) {
		long sfi;
		int i, x;
		int buflen;
		int bufbytes;
		int chancount;
		int samplecount;
		int samplerate;
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
				samplerate = sfp->Frequency();
				samplecount = samplerate * 2; // 2 sec buffers
				buflen = chancount * samplecount;
				bufbytes = buflen * 2;
				this->codec.Rewind(*sfp);
				buf = new unsigned short[buflen];
				ALDEBUG(std::cerr << "ALSound: Buffering Ch:" << chancount << " R:" << samplerate << " sz:" << bufbytes << "\n";)
				x = 0;
				while(x < this->buffercount && 0 < (i = codec.FetchBuffer(*sfp, buf, ((chancount == 1) ? resource::PCM_MONO16 : resource::PCM_STEREO16), samplecount))) {
					albuf[x] = master->buffers[this->buffers[x]]->GetID();
					if(i < samplecount) {
						ALDEBUG(std::cerr << "ALSound: Buffered " << x << "b + " << i << '/' << samplecount << "samples\n";)
						buflen = chancount * i;
						stream = false;
						bufbytes = buflen * sizeof(short);
						alBufferData(albuf[x], ((chancount == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16), buf, bufbytes, samplerate);
						break;
					}
					else {
						alBufferData(albuf[x], ((chancount == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16), buf, bufbytes, samplerate);
						stream = true;
					}
					x++;
				}
				if(x < this->buffercount) {
					stream = false;
					ALDEBUG(std::cerr << "ALSound: Only " << x << '/' << this->buffercount << " buffers used, not streaming.\n";)
				}
				delete buf;
				if(x > 0) {
					alSourceQueueBuffers(this->sourceid, x, albuf);
					alSourcePlay(this->sourceid);
					if(playloop == PLAYBACK_LOOP && !stream) {
						alSourcei(this->sourceid, AL_LOOPING, AL_TRUE);
						ALDEBUG(std::cerr << "ALSound: Looping playback\n";)
					} else {
						alSourcei(this->sourceid, AL_LOOPING, AL_FALSE);
						ALDEBUG(std::cerr << "ALSound: NOT looping playback, stream:" << stream << '\n';)
					}
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
	void ALSound::PlayMode(ORDERING o, PLAYBACK mode) {
		if(o != ORDERING_NONE) { playorder = o; }
		if(mode != PLAYBACK_NONE) { playloop = mode; }
	}

	void ALSound::Gain(float mul) {
		alSourcef(this->sourceid, AL_GAIN,mul);
	}
	void ALSound::Rolloff(float atten) {
		alSourcef(this->sourceid, AL_ROLLOFF_FACTOR, atten);
	}
	void ALSound::ReferenceDistance(float r) {
		alSourcef(this->sourceid, AL_REFERENCE_DISTANCE, r);
	}
	void ALSound::MaxDistance(float r) {
		alSourcef(this->sourceid, AL_MAX_DISTANCE, r);
	}
	void ALSound::Pitch(float r) {
		alSourcef(this->sourceid, AL_PITCH, r);
	}

	void ALSound::Position(float x, float y, float z) {
		alSource3f(this->sourceid, AL_POSITION,x,y,z);
	}
	void ALSound::Position(glm::vec3 v) {
		alSource3f(this->sourceid, AL_POSITION,v.x,v.y,v.z);
	}
	void ALSound::Velocity(float x, float y, float z) {
		alSource3f(this->sourceid, AL_VELOCITY,x,y,z);
	}
	void ALSound::Velocity(glm::vec3 v) {
		alSource3f(this->sourceid, AL_VELOCITY,v.x,v.y,v.z);
	}
} // Namespace Sigma
