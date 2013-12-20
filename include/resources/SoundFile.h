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
			void LoadFromDisk(std::string fn) {
				std::ifstream::pos_type sz;
				FourCC fourcc;

				// read from file system
				std::ifstream fh(fn, std::ios::in|std::ios::binary|std::ios::ate);
				if(fh.is_open()) {
					sz = fh.tellg(); // get file size
					fh.seekg(0, std::ios::beg); // goto beginning
					fh.read(fourcc.cvalue, 4); // read the id string
					fh.seekg(0, std::ios::beg);
					if(fourcc == FourCC('R','I','F','F')) {
						LoadWAV(fh, sz);
					}
					else if(fourcc == FourCC('O','g','g','S')) {
						LoadOgg(fh, sz);
					}
					else {
						data = new unsigned char[sz];
						fh.seekg(0, std::ios::beg);
						fh.read((char*)&(*data), sz);
					}
					fh.close();
				}
			}
			void LoadWAV(std::ifstream &fh, std::ifstream::pos_type sz) {
			}
			void LoadOgg(std::ifstream &fh, std::ifstream::pos_type sz) {
			}
		private:
			unsigned char* data;
		};
	}
}