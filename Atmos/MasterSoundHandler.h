#pragma once

#include "Volume.h"

namespace Atmos
{
    class MasterSoundHandler
    {
    private:
        static Volume masterVolume;

        MasterSoundHandler() = default;
        MasterSoundHandler(const MasterSoundHandler &arg) = delete;
        MasterSoundHandler& operator=(const MasterSoundHandler &arg) = delete;
    public:
        static MasterSoundHandler& Instance();
        static void Init();
        // Value between 0 and 100
        static void SetMasterVolume(Volume set);
        static Volume GetMasterVolume();
    };
}