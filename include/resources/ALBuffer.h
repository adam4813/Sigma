#pragma once

#include <AL/al.h>
#include <math.h>
#include "resources/SoundFile.h"

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
            void SineSynth(float f) {
                int i;
                float x;
                unsigned char * buf = new unsigned char[11025];
                for(i = 0; i < 11025; i++) {
                    x = i * f;
                    buf[i] = (unsigned char)(128.0f + 64.f * sin(x * 3.14159679f));
                }
                alBufferData(this->id, AL_FORMAT_MONO8, buf, 11025, 44100);
                delete buf;
            }
            ALuint GetID() const { return id; }
        private:
            ALuint id;
        };
    }
}
