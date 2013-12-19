#pragma once

#include <AL/al.h>
#include <math.h>

namespace Sigma {
	namespace resource {
		class ALBuffer {
		public:
			ALBuffer() : id(0) { }

			bool GenerateBuffer() {
				alGenBuffers(1, &this->id);
				return (alIsBuffer(this->id) == AL_TRUE);
			}
			void DeleteBuffer() {
				alDeleteBuffers(1, &this->id);
				this->id = 0;
			}
			void LoadTestPattern() {
				int i;
				float f;
				unsigned char * buf = new unsigned char[5625];
				for(i = 0; i < 5625; i++) {
					f = i * 0.1f;
					buf[i] = (unsigned char)(128.0f + 32.f * sin(f));
				}
				alBufferData(this->id, AL_FORMAT_MONO8, buf, 5625, 22500);
			}
			ALuint GetID() const { return id; }
		private:
			ALuint id;
		};
	}
}
