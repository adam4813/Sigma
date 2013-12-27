#pragma once

#include <memory>
#include <vector>
#include "IComponent.h"
#include "resources/SoundFile.h"

namespace Sigma {

    enum PLAYBACK {
        PLAYBACK_NONE = -1,
        PLAYBACK_NORMAL = 0,
        PLAYBACK_LOOP = 1
    };

    enum ORDERING {
        ORDERING_NONE = -1,
        ORDERING_SEQUENCIAL = 0,
        // lots of synonyms
        ORDERING_SEQ = 0,
        ORDERING_NORMAL = 0,
        ORDERING_ORDERED = 0,
        ORDERING_RANDOM = 1,
        ORDERING_UNORDERED = 1
    };

    class ISound : public IComponent {
    public:
        ISound(int entityID) : IComponent(entityID), paused(false), playing(false), playindex(0), playorder(ORDERING_SEQ), playloop(PLAYBACK_NORMAL) {}
        virtual ~ISound() {}

        virtual void Play(PLAYBACK mode = PLAYBACK_NONE) = 0;
        virtual void Pause() = 0;
        virtual void Stop() = 0;
        virtual void PlayMode(ORDERING o = ORDERING_NONE, PLAYBACK mode = PLAYBACK_NONE) {
            if(o != PLAYBACK_NONE) { playorder = o; }
            if(mode != ORDERING_NONE) { playloop = mode; }
        }
        virtual void AddSound(long i) {
            playlist.push_back(i);
        }
        virtual void ClearSounds() {
            playlist.clear();
        }
    protected:
        std::vector<long> playlist;
        int playindex;
        ORDERING playorder;
        PLAYBACK playloop;
        bool playing;
        bool paused;
    };
} // namespace Sigma
