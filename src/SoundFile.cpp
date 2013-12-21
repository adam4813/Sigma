
#include "resources/SoundFile.h"

namespace Sigma {
	namespace resource {

		void SoundFile::LoadWAV(std::ifstream &fh, std::ifstream::pos_type sz) {
		}
		void SoundFile::LoadOgg(std::ifstream &fh, std::ifstream::pos_type sz) {
		}
		void SoundFile::LoadFromFile(std::string fn) {
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
	}
}
