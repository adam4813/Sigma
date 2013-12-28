#pragma once

#include <memory>
#include <fstream>
#include <math.h>

namespace Sigma {
	namespace resource {
		// A class to hold an audio stream in its (possibly compressed) raw format
		enum AUDIO_CODEC {
			Null,
			PCM,
			Vorbis,
			RIFF,
		};

		enum AUDIO_PCM_FORMAT {
			PCM_MONO8,
			PCM_MONO16,
			PCM_MONO24,
			PCM_MONOf32,
			PCM_STEREO8,
			PCM_STEREO16,
			PCM_STEREO24,
			PCM_STEREOf32,
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
		class SoundFile; // forward declare

		class Decoder {
		public:
			Decoder();
			~Decoder();
			void Rewind(SoundFile &sf);
			bool EndOfStream();
			int FetchBuffer(SoundFile &, void * out, AUDIO_PCM_FORMAT fmt, long count);
			int FetchBuffer(SoundFile &, void * out, AUDIO_PCM_FORMAT fmt, long count, int freq);
			int Frequency(SoundFile &);

			/*
			 * Converts between sample formats
			 */
			static void * Resample(void * out, AUDIO_PCM_FORMAT outfmt, void * in, AUDIO_PCM_FORMAT infmt, long count);
			
			/*
			 * Interlaces two channels while converting sample formats
			 */
			static void * MergeSample(void * out, AUDIO_PCM_FORMAT outfmt, void ** inc, AUDIO_PCM_FORMAT infmt, long count);
			void ProcessMeta(SoundFile &);
		protected:
			void * decoderstate;
			bool hasmeta;
			bool decoderinit;
		};

		class SoundFile {
			friend class Decoder;
		public:
			SoundFile() : data(nullptr), dataformat(Null), pcmsize(PCM_MONO16), freq(0), chann(0), hasmeta(false) { }
			~SoundFile();
			bool isLoaded() { return (this->data != nullptr); }
			void LoadFromFile(std::string fn);
			void LoadWAV(std::ifstream &fh, std::ifstream::pos_type sz);
			void LoadOgg(std::ifstream &fh, std::ifstream::pos_type sz);
			AUDIO_CODEC Format() { return dataformat; }
			bool isStream() { return true; }
			int Frequency() {
				if(hasmeta) {
					return freq;
				} else {
					ProcessMeta();
					return freq;
				}
			}
			int Channels() {
				if(hasmeta) {
					return chann;
				} else {
					ProcessMeta();
					return chann;
				}
			}
			void ProcessMeta() {
				Decoder d;
				d.ProcessMeta(*this);
			}
		protected:
			unsigned char* data;
			AUDIO_CODEC dataformat;
			AUDIO_PCM_FORMAT pcmsize;
			int freq;
			int chann;
			bool hasmeta;
		};
	}
}
