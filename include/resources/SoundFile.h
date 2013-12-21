#pragma once

#include <memory>
#include <fstream>
#include <math.h>

namespace Sigma {
	namespace resource {
		// A class to hold an audio stream in its (possibly compressed) raw format
		enum AUDIO_FORMAT {
			Null,
			PCM,
			Vorbis
		};

		struct FourCC {
			union {
				unsigned long lvalue;
				char cvalue[4];
			};
			FourCC() {}
			FourCC(char a, char b, char c, char d) {
				cvalue[0] = a; cvalue[1] = b; cvalue[2] = c; cvalue[3] = d;
			}
			bool operator==(FourCC &rhv) {
				return lvalue == rhv.lvalue;
			}
		};

		class SoundFile {
		public:
			SoundFile() : data(nullptr) { }
			~SoundFile() {
				if(data != nullptr) {
					delete data;
				}
			}
			bool isLoaded() { return (this->data != nullptr); }
			void LoadFromFile(std::string fn);
			void LoadWAV(std::ifstream &fh, std::ifstream::pos_type sz);
			void LoadOgg(std::ifstream &fh, std::ifstream::pos_type sz);
		private:
			unsigned char* data;
		};
	}
}
