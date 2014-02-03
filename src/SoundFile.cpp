
extern "C" {
#include <ogg/ogg.h>
#include <vorbis/codec.h>
}
#include <memory>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include "resources/SoundFile.h"
#include "systems/OpenALSystem.h"

#include "Sigma.h"

struct OggLinkedPacket {
	ogg_packet pack;
	void * next;
};

struct VorbisState {
	vorbis_info info;
	vorbis_comment com;
	vorbis_dsp_state dsp;
	vorbis_block block;
	OggLinkedPacket * obj;
};

namespace Sigma {
	namespace resource {

		struct RIFFChunk {
			FourCC id;
			unsigned long size;
		};
		struct WAVEHeader {
			unsigned short format;
			unsigned short channels;
			unsigned long samplerate;
			unsigned long byterate;
			unsigned short align;
			unsigned short samplebits;
		};

		SoundFile::~SoundFile() {
			if(data != nullptr) {
				free( data );
			}
		}

		// Note: WAV files can get huge!
		// ogg can offer basically the same quality in less space
		// both in memory and on disk.
		void SoundFile::LoadWAV(std::ifstream &fh, std::ifstream::pos_type sz) {
			RIFFChunk chk;
			FourCC ffid;
			WAVEHeader head;
			unsigned long readcount;
			fh.read((char*)&chk, sizeof(RIFFChunk));
			if(chk.id == FourCC('R','I','F','F')) {
				readcount = chk.size;
				fh.read((char*)&ffid, sizeof(FourCC));
				readcount -= sizeof(FourCC);
				if(ffid == FourCC('W','A','V','E')) {
					dataformat = RIFF;
					while(!fh.eof() && readcount) {
						fh.read((char*)&chk, sizeof(RIFFChunk));
						readcount -= sizeof(RIFFChunk);
						if(chk.id == FourCC('f','m','t',' ')) {
							fh.read((char*)&head, sizeof(WAVEHeader));
							readcount -= chk.size;
							if(chk.size > sizeof(WAVEHeader)) {
								fh.ignore(chk.size - sizeof(WAVEHeader));
							}
						}
						else if(chk.id == FourCC('d','a','t','a')) {
							if(this->data) { free(data); }
							this->data = (unsigned char*)malloc(sizeof(WAVEHeader) + 4 + chk.size);
							if(this->data == nullptr) {
								LOG_ERROR << errno << "] Memory can not allocate " << sizeof(WAVEHeader) + 4 + chk.size << " bytes";
								return;
							}
							*((unsigned long*)this->data) = chk.size / (head.align);
							memcpy(this->data + 4, &head, sizeof(WAVEHeader));
							fh.read((char*)this->data + 4 + sizeof(WAVEHeader), chk.size);
							readcount -= chk.size;
						}
						else {
							LOG_DEBUG << "[WAV] Skip chunk " << chk.id.cvalue[0] << chk.id.cvalue[1] << chk.id.cvalue[2] << chk.id.cvalue[3];
							fh.ignore(chk.size); // unknown chunks
						}
					}
				}
			}
		}
		void SoundFile::LoadOgg(std::ifstream &fh, std::ifstream::pos_type sz) {
			ogg_sync_state sync;
			ogg_stream_state stream;
			ogg_page page;
			unsigned long offs;
			size_t datasz;
			size_t allosz;
			size_t ndatasz;
			int serialnum = 0;
			unsigned char * redata;
			char * oggbuf;
			OggLinkedPacket * opdata;

			datasz = sizeof(OggLinkedPacket);
			allosz = (datasz) + 0x4000 - (datasz & 0x3fff);
			offs = 0;
			if(this->data) { free(data); }
			this->data = (unsigned char*)malloc(allosz);
			if(this->data == nullptr) {
				LOG_ERROR << errno << "] Memory can not allocate " << allosz << " bytes";
				return;
			}
			opdata = (OggLinkedPacket*)this->data;

			ogg_sync_init(&sync);
			
			while(!fh.eof()) {
				if(ogg_sync_pageout(&sync, &page) != 1) {
					oggbuf = ogg_sync_buffer(&sync, 0x4000);
					if(oggbuf) {
						fh.read(oggbuf, 0x4000);
						ogg_sync_wrote(&sync, (long)fh.gcount());
					}
				} else {
					if(!serialnum) {
						serialnum = ogg_page_serialno(&page);
						ogg_stream_init(&stream, serialnum);
					}
					if(ogg_stream_pagein(&stream, &page) == 0) {
						while(ogg_stream_packetout(&stream, &opdata->pack) == 1) {
							long bytec;
							unsigned char * packdata;
							packdata = opdata->pack.packet;
							bytec = opdata->pack.bytes;
							if(!offs) {
								if(vorbis_synthesis_idheader(&opdata->pack) == 1) {
									LOG_DEBUG << "Vorbis";
									dataformat = Vorbis;
								}
							}
							// expand packet buffer
							ndatasz = datasz + bytec + sizeof(OggLinkedPacket);
							if(ndatasz > allosz) {
								allosz = ((ndatasz+1) - ((ndatasz+1) & 0x3fff)) + 0x8000;
								redata = (unsigned char*)realloc(this->data, allosz);
							} else {
								redata = this->data;
							}
							if(redata != nullptr) {
								this->data = redata;
								opdata = (OggLinkedPacket*)(data + offs);
								opdata->next = nullptr;
								// copy to buffer after packet
								memcpy((this->data + offs + sizeof(OggLinkedPacket)), packdata, bytec);
								// move to next packet
								offs += sizeof(OggLinkedPacket) + bytec;
								opdata = (OggLinkedPacket*)(this->data + offs);
								datasz = ndatasz;
							} else {
								LOG_WARN << errno << "] Memory can not reallocate " << allosz << " bytes";
							}
						}
					}
				}
			}

			// build pointers & linked list
			OggLinkedPacket *lastlp;
			opdata = nullptr;
			for(offs = 0; offs < datasz; ) {
				lastlp = opdata;
				opdata = (OggLinkedPacket*)(data + offs);
				opdata->pack.packet = (data + offs + sizeof(OggLinkedPacket));
				offs += sizeof(OggLinkedPacket);
				if(offs < datasz) { offs += opdata->pack.bytes; } // avoid random data overflows
				opdata->next = (OggLinkedPacket*)(data + offs);
			}
			if(lastlp) { lastlp->next = nullptr; }

			LOG_DEBUG << datasz << " bytes loaded.";
			ogg_stream_clear(&stream);
			ogg_sync_clear(&sync);
		}
		Decoder::Decoder() : decoderstate(nullptr), decoderinit(false), hasmeta(false) {
		}
		Decoder::~Decoder() {
			if(this->decoderstate) { free(this->decoderstate); }
		}
		void Decoder::Rewind(SoundFile &sf) {
			VorbisState *vs;
			
			switch(sf.dataformat) {
			case Vorbis:
				if(this->decoderinit) {
					vs = (VorbisState*)this->decoderstate;
					if(vorbis_synthesis_restart(&vs->dsp) == 0) {
					}
					vs->obj = (OggLinkedPacket*)sf.data;
				}
				break;
			case RIFF:
				unsigned long *rs;
				rs = (unsigned long *)this->decoderstate;
				if(rs == nullptr) {
					this->decoderstate = rs = static_cast<unsigned long*>(malloc(sizeof(unsigned long) * 2));
				}
				rs[0] = 0;
				rs[1] = 0;
				break;
			}
		}
		bool Decoder::EndOfStream() {
			return true;
		}
		int Decoder::FetchBuffer(SoundFile &sf, void * out, AUDIO_PCM_FORMAT fmt, long count) {
			if(!this->hasmeta) {
				ProcessMeta(sf);
			}
			VorbisState *vs;
			int i, samples;
			int readsamples;
			float **fbp;
			switch(sf.dataformat) {
			case Vorbis:
				vs = (VorbisState*)this->decoderstate;
				if(!this->decoderinit) {
					if(vorbis_synthesis_init(&vs->dsp, &vs->info) == 0) {
						vorbis_block_init(&vs->dsp, &vs->block);
						this->decoderinit = true;
					}
				}
				if(this->decoderinit) {
					readsamples = 0;
					while(count > readsamples) {
						samples = vorbis_synthesis_pcmout(&vs->dsp, &fbp);
						if(samples == 0) {
							if(vs->obj) {
								if(!(i=vorbis_synthesis(&vs->block, &vs->obj->pack))) {
									i = vorbis_synthesis_blockin(&vs->dsp, &vs->block);
									if(i < 0) {
										i = 0;
									}
								} else {
									i = 0;
								}
								vs->obj = (OggLinkedPacket*)vs->obj->next;
							} else {
								return 0;
							}
						} else {
							if(readsamples + samples > count) {
								samples = count - readsamples;
							}
							out = MergeSample(out, fmt, (void**)fbp, sf.pcmsize, samples);
							vorbis_synthesis_read(&vs->dsp, samples);
							readsamples += samples;
						}
					}
					return readsamples;
				}
				break;
			case RIFF:
				{
				WAVEHeader *head;
				unsigned long *rs;
				unsigned long filelen;
				unsigned char * pcmdat;
				rs = (unsigned long *)this->decoderstate;
				if(rs == nullptr) {
					this->decoderstate = rs = static_cast<unsigned long*>(malloc(sizeof(unsigned long) * 2));
				}
				if(!this->decoderinit) {
					rs[0] = 0;
					rs[1] = 0;
					this->decoderinit = true;
				}
				filelen = *((unsigned long*)sf.data);
				head = (WAVEHeader *)(sf.data + 4);
				pcmdat = (unsigned char *)(sf.data + 4 + sizeof(WAVEHeader));

				samples = count;
				if(samples + rs[0] > filelen) {
					samples = filelen - rs[0];
				}
				if(samples > 0) {
					out = Resample(out, fmt, pcmdat+(rs[0]*head->align), sf.pcmsize, samples);
					rs[0] += samples;
				}
				return samples;
				}
				break;
			default:
				break;
			}
			return 0;
		}

		int Decoder::FetchBuffer(SoundFile &sf, void * out, AUDIO_PCM_FORMAT fmt, long count, int freq) {
			return 0; // Rate conversion is not supported
		}
		int Decoder::Frequency(SoundFile &sf) {
			if(sf.hasmeta) {
				return sf.freq;
			}
			return 0;
		}
		void Decoder::ProcessMeta(SoundFile &sf) {
			if(this->hasmeta) { return; }
			switch(sf.dataformat) {
			case Vorbis:
				{
				this->decoderstate = malloc(sizeof(VorbisState));
				VorbisState * vs = static_cast<VorbisState*>(this->decoderstate);
				vorbis_info_init(&vs->info);
				vorbis_comment_init(&vs->com);
				vs->obj = (OggLinkedPacket*)sf.data;
				int i = 0;
				while((i++ < 3) && vs->obj != nullptr && vorbis_synthesis_headerin(&vs->info, &vs->com, &vs->obj->pack) == 0) {
					vs->obj = (OggLinkedPacket*)vs->obj->next;
				}
				this->hasmeta = true;
				if(!sf.hasmeta) {
					sf.freq = vs->info.rate;
					sf.chann = vs->info.channels;
					sf.pcmsize = (sf.chann != 1 ? PCM_STEREOf32 : PCM_MONOf32);
					sf.hasmeta = true;
				}
				}
				break;
			case RIFF:
				{
				WAVEHeader *head;
				this->decoderstate = malloc(sizeof(unsigned long) * 2);
				head = (WAVEHeader *)(sf.data + 4);
				this->hasmeta = true;
				if(!sf.hasmeta) {
					sf.freq = head->samplerate;
					sf.chann = head->channels;
					sf.pcmsize = (AUDIO_PCM_FORMAT)(((sf.chann - 1) << 2) | (((head->samplebits >> 3)-1) & 0x3));
					sf.hasmeta = true;
				}
				}
				break;
			case PCM:
				sf.hasmeta = true;
				break;
			default:
				sf.hasmeta = true;
				break;
			}
		}
		void * Decoder::MergeSample(void * out, AUDIO_PCM_FORMAT outfmt, void ** inc, AUDIO_PCM_FORMAT infmt, long count) {
			int inchanbytes;
			int outchanbytes;
			int inchannels;
			//unsigned char *cidat, *codat;
			//short *sidat;
			short *sodat;
			//unsigned long *lidat, *lodat;
			float *fildat, *firdat;
			//float *fodat;
			long k;
			// AUDIO_PCM_FORMAT defines x in lower 2 bits (x+1 = bytes per channel)
			//  and y in remaining upper bits (y+1 = channels)
			inchannels = 1 + (infmt >> 2);
			inchanbytes = 1 + (infmt & 3);
			outchanbytes = 1 + (outfmt & 3);
			switch(inchanbytes) {
			case 1:
				return out;
			case 2:
				return out;
			case 3: // int 24
				switch(outchanbytes) {
					case 1:
						return out;
					case 2: // int 16
						return out;
					case 3: // int 24
						return out;
					case 4: // float 32
						return out;
				}
			case 4: // float 32
				switch(outchanbytes) {
					case 1:
						return out;
					case 2: // int 16
						fildat = (float*)inc[0];
						firdat = (float*)inc[1];
						sodat = (short*)out;
						for(k = 0; k < count; k++) {
							*(short*)sodat = static_cast<short>(fildat[k] * 16383.f);
							sodat++;
							if(inchannels == 2) {
								*(short*)sodat = static_cast<short>(firdat[k] * 16383.f);
								sodat++;
							}
						}
						return sodat;
					case 3: // int 24
						return out;
					case 4: // float 32
						return out;
				}
			}
			return out;
		}
		void * Decoder::Resample(void * out, AUDIO_PCM_FORMAT outfmt, void * in, AUDIO_PCM_FORMAT infmt, long count) {
			int inchanbytes;
			int outchanbytes;
			int inchannels;
			int outchannels;
			unsigned char *cidat;
			short *sodat;
			//unsigned long *lidat, *lodat;
			float *fidat, *fodat;
			bool chanadd = false;
			bool chancomp = false;
			float fmax = 1.0f, fmin = -1.0f;
			long fcount;
			long k;
			// AUDIO_PCM_FORMAT defines x in lower 2 bits (x+1 = bytes per channel)
			//  and y in remaining upper bits (y+1 = channels)
			inchannels = 1 + (infmt >> 2);
			inchanbytes = 1 + (infmt & 3);
			outchannels = 1 + (outfmt >> 2);
			outchanbytes = 1 + (outfmt & 3);
			if(infmt == outfmt) {
				memmove(out, in, count * inchanbytes * inchannels);
				return ((char*)out) + (count * inchanbytes * inchannels);
			}
			fcount = count * inchannels;
			if(inchannels > outchannels) { chancomp = true; }
			if(outchannels > inchannels) { chanadd = true; }
			switch(inchanbytes) {
			case 1:
				return out;
			case 2:
				return out;
			case 3: // int 24
				switch(outchanbytes) {
					case 1:
						return out;
					case 2: // int 16
						k = 0;
						for(cidat = (unsigned char *)in, sodat = (short*)out; k++ < fcount; cidat+=3, sodat++) {
							*((unsigned short*)sodat) = (unsigned short)((*((unsigned long*)cidat)) >> 8);
							if(chanadd) { cidat-=3; }
							if(chancomp) { cidat+=3; }
						}
						return sodat;
					case 3: // int 24
						return out;
					case 4: // float 32
						return out;
				}
			case 4: // float 32
				switch(outchanbytes) {
					case 1:
						return out;
					case 2: // int 16
						k = 0;
						for(fidat = (float*)in, sodat = (short*)out; k++ < fcount; fidat++, sodat++) {
							*(unsigned short*)sodat = static_cast<unsigned short>(((*fidat)+1.0f) * 16383.f);
							if(*fidat < fmin) { LOG << "m" << *fidat; sodat--; }
							if(*fidat > fmax) { LOG << "x" << *fidat; sodat--; }
							if(chanadd) { fidat--; }
							if(chancomp) { fidat++; }
						}
						return sodat;
					case 3: // int 24
						return out;
					case 4: // float 32
						for(fidat = (float*)in, fodat = (float*)out; fcount-- >= 0; fidat++, fodat++) {
							*fodat = *fidat;
							if(chanadd) { fidat--; }
							if(chancomp) { fidat++; }
						}
						return fodat;
				}
			}
			return out;
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
					LOG << "Loading Sound from WAV file: " << fn;
					LoadWAV(fh, sz);
					ProcessMeta();
				}
				else if(fourcc == FourCC('O','g','g','S')) {
					LOG << "Loading Sound from Ogg file: " << fn;
					LoadOgg(fh, sz);
					ProcessMeta();
				}
				else {
					data = (unsigned char*)malloc( (size_t)sz );
					if(data) {
						fh.seekg(0, std::ios::beg);
						fh.read((char*)data, sz);
					}
				}
				fh.close();
			}
		}
	}
}
