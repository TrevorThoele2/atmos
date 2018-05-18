
#include "MasterSoundHandler.h"

#include "Environment.h"

namespace Atmos
{
    Volume MasterSoundHandler::masterVolume = 0;

    MasterSoundHandler& MasterSoundHandler::Instance()
    {
        static MasterSoundHandler instance;
        return instance;
    }

    void MasterSoundHandler::Init()
    {
        SetMasterVolume(*Environment::GetIni().GetEntry<Ini::ID::MASTER_SOUND>());
    }

    void MasterSoundHandler::SetMasterVolume(Volume set)
    {
        if (Environment::GetAudio()->SetMasterVolume(set))
            masterVolume = set;
    }

    Volume MasterSoundHandler::GetMasterVolume()
    {
        return masterVolume;
    }
}