
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

struct VorbisState {
    vorbis_info info;
    vorbis_comment com;
    vorbis_dsp_state dsp;
    vorbis_block block;
    OggLinkedPacket * obj;
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

            std::cerr << datasz << " bytes loaded.";
            ogg_stream_clear(&stream);
            ogg_sync_clear(&sync);
        }
        Decoder::Decoder() : decoderstate(nullptr), decoderinit(false), hasmeta(false) {
        }
        Decoder::~Decoder() {
            if(this->decoderstate) { delete this->decoderstate; }
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
                            out = MergeSample(out, fmt, fbp[0], fbp[1], sf.pcmsize, samples);
                            vorbis_synthesis_read(&vs->dsp, samples);
                            readsamples += samples;
                        }
                    }
                    return readsamples;
                }
                break;
            default:
                break;
            }
            return 0;
        }
        int Decoder::FetchBuffer(SoundFile &sf, void * out, AUDIO_PCM_FORMAT fmt, long count, int freq) {
            return 0;
        }
        int Decoder::Frequency(SoundFile &sf) {
            return 0;
        }
        void Decoder::ProcessMeta(SoundFile &sf) {
            switch(sf.dataformat) {
            case Vorbis:
                {
                this->decoderstate = new VorbisState;
                VorbisState * vs = (VorbisState*)this->decoderstate;
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
            case PCM:
                sf.hasmeta = true;
                break;
            default:
                sf.hasmeta = true;
                break;
            }
        }
        void * Decoder::MergeSample(void * out, AUDIO_PCM_FORMAT outfmt, void * inl, void * inr, AUDIO_PCM_FORMAT infmt, long count) {
            int inchanbytes;
            int outchanbytes;
            //unsigned char *cidat, *codat;
            //short *sidat;
            short *sodat;
            //unsigned long *lidat, *lodat;
            float *fildat, *firdat;
            //float *fodat;
            long k;
            // AUDIO_PCM_FORMAT defines x in lower 2 bits (x+1 = bytes per channel)
            //  and y in remaining upper bits (y+1 = channels)
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
                        fildat = (float*)inl;
                        firdat = (float*)inr;
                        sodat = (short*)out;
                        for(k = 0; k++ < count; firdat++, fildat++) {
                            *(short*)sodat = static_cast<short>(((*fildat)) * 16383.f);
                            sodat++;
                            *(short*)sodat = static_cast<short>(((*firdat)) * 16383.f);
                            sodat++;
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
                        for(cidat = (unsigned char *)in, sodat = (short*)out; fcount-- >= 0; cidat+=3, sodat++) {
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
                            if(*fidat < fmin) { std::cerr << "m" << *fidat; sodat--; }
                            if(*fidat > fmax) { std::cerr << "x" << *fidat; sodat--; }
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
                    std::cerr << "Loading Sound from WAV file: " << fn << '\n';
                    LoadWAV(fh, sz);
                }
                else if(fourcc == FourCC('O','g','g','S')) {
                    std::cerr << "Loading Sound from Ogg file: " << fn;
                    LoadOgg(fh, sz);
                    ProcessMeta();
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
