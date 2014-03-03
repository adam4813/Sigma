
#include "components/ALSound.h"

namespace Sigma {
	ALSound::ALSound(id_t entityID,OpenALSystem *m)
		: ISound(entityID), buffercount(0), bufferindex(0), bufferloaded(0), master(m), sourceid(0), stream(false) { }
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
				if(master->GetSoundFile(sfi).expired()) {
					return;
				}
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
						LOG_DEBUG1 << "ALSound: Buffer-set wrap around";
					}
					i = codec.FetchBuffer(*sfp, buf, ((chancount == 1) ? resource::PCM_MONO16 : resource::PCM_STEREO16), samplecount);
					alSourceUnqueueBuffers(this->sourceid, 1, &albuf);
					if(i > 0) {
						if(i < samplecount) {
							buflen = chancount * i;
							if(playloop == PLAYBACK_LOOP) {
								this->codec.Rewind(*sfp);
								LOG_DEBUG1 << "ALSound: Looping stream at sample: " << i;
								//bufbytes = i;
								//i = codec.FetchBuffer(*sfp, buf+bufbytes, ((chancount == 1) ? resource::PCM_MONO16 : resource::PCM_STEREO16), samplecount - i);
								//buflen = bufbytes+i;
								buflen = i;
								buflen *= chancount;
							} else {
								LOG_DEBUG1 << "ALSound: Playback done; at sample: " << i;
								playing = false;
							}
							bufbytes = buflen * sizeof(short);
							LOG_DEBUG1 << "ALSound: Buffered " << albuf << "b + " << i << '/' << samplecount << " samples";
							alBufferData(albuf, ((chancount == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16), buf, bufbytes, samplerate);
						}
						else {
							LOG_DEBUG1 << "ALSound: Buffered " << albuf << "b + " << i << '/' << samplecount << " samples";
							alBufferData(albuf, ((chancount == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16), buf, bufbytes, samplerate);
						}
						alSourceQueueBuffers(this->sourceid, 1, &albuf);
					} else {
						LOG_DEBUG1 << "ALSound: Playback end " << albuf << "b + " << i << '/' << samplecount << "samples";
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
				LOG_DEBUG1 << "ALSound: Playback underrun";
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
		if(playlist.size() > 0 && (stream || !bufferloaded)) {
			if(playindex > playlist.size()) { playindex = 0; }
			sfi = playlist[playindex];
			if(!master->GetSoundFile(sfi).expired()) {
				sfp = std::shared_ptr<resource::SoundFile>(master->GetSoundFile(sfi));
			}
			else {
				return;
			}
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
				LOG_DEBUG1 << "ALSound: Buffering Ch:" << chancount << " R:" << samplerate << " sz:" << bufbytes;
				x = 0;
				while(x < this->buffercount && 0 < (i = codec.FetchBuffer(*sfp, buf, ((chancount == 1) ? resource::PCM_MONO16 : resource::PCM_STEREO16), samplecount))) {
					albuf[x] = master->buffers[this->buffers[x]]->GetID();
					if(i < samplecount) {
						LOG_DEBUG1 << "ALSound: Buffered " << x << "b + " << i << '/' << samplecount << "samples";
						buflen = chancount * i;
						stream = false;
						bufbytes = buflen * sizeof(short);
						if(playloop == PLAYBACK_LOOP) {
							codec.Rewind(*sfp);
							stream = true; // better looping results
						}
						alBufferData(albuf[x], ((chancount == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16), buf, bufbytes, samplerate);
						//break;
					}
					else {
						LOG_DEBUG1 << "ALSound: Buffered " << x << "b + " << i << '/' << bufbytes;
						alBufferData(albuf[x], ((chancount == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16), buf, bufbytes, samplerate);
						stream = true;
					}
					x++;
				}
				if(x < this->buffercount) {
					stream = false;
					LOG_DEBUG1 << "ALSound: Only " << x << '/' << this->buffercount << " buffers used, not streaming.";
				}
				delete buf;
				if(x > 0) {
					bufferloaded = x;
					alSourceUnqueueBuffers(this->sourceid, this->buffercount, albuf);
					alSourceQueueBuffers(this->sourceid, x, albuf);
					alSourcePlay(this->sourceid);
					if(playloop == PLAYBACK_LOOP && !stream) {
						alSourcei(this->sourceid, AL_LOOPING, AL_TRUE);
						LOG_DEBUG1 << "ALSound: Looping playback";
					} else {
						alSourcei(this->sourceid, AL_LOOPING, AL_FALSE);
						LOG_DEBUG1 << "ALSound: NOT looping playback, stream:" << stream;
					}
					playing = true;
					paused = false;
				}
				return;
			}
		}
		else if(playlist.size() > 0 && (bufferloaded) && !stream) {
			alSourcePlay(this->sourceid);
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
	void ALSound::Relative(bool r) {
		alSourcei(this->sourceid, AL_SOURCE_RELATIVE, (r? AL_TRUE:AL_FALSE));
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
