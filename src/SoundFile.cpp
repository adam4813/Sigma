
extern "C" {
#include <ogg/ogg.h>
#include <vorbis/codec.h>
}
#include <memory>
#include <cstdlib>
#include <iostream>
#include <string>
#include "resources/SoundFile.h"

struct OggLinkedPacket {
	ogg_packet pack;
	void * next;
};

namespace Sigma {
	namespace resource {

		SoundFile::~SoundFile() {
			if(data != nullptr) {
				free( data );
			}
		}
		void SoundFile::LoadWAV(std::ifstream &fh, std::ifstream::pos_type sz) {
		}
		void SoundFile::LoadOgg(std::ifstream &fh, std::ifstream::pos_type sz) {
			ogg_sync_state sync;
			ogg_stream_state stream;
			ogg_page page;
			long offs;
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
			if(this->data == nullptr) { return; }
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
									std::cerr << "\nVorbis ";
								}
							}
							// expand packet buffer
							ndatasz = datasz + bytec + sizeof(OggLinkedPacket);
							if(ndatasz > allosz) {
								allosz = ((ndatasz+1) - ((ndatasz+1) & 0x3fff)) + 0x4000;
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
							}
						}
					}
				}
			}
			std::cerr << datasz << " bytes loaded.";
			ogg_stream_clear(&stream);
			ogg_sync_clear(&sync);
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
					std::cerr << "Loading Sound from WAV file: " << fn << '\n';
					LoadWAV(fh, sz);
				}
				else if(fourcc == FourCC('O','g','g','S')) {
					std::cerr << "Loading Sound from Ogg file: " << fn;
					LoadOgg(fh, sz);
					std::cerr << '\n';
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
