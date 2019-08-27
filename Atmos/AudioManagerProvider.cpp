#include "AudioManagerProvider.h"

#include "NullAudioManager.h"

namespace Atmos::Audio
{
    AudioManagerProvider::AudioManagerProvider()
    {
        value = std::make_unique<NullAudioManager>();
    }
}

namespace Arca
{
    const TypeName Traits<Atmos::Audio::AudioManagerProvider>::typeName = "AudioManagerProvider";
}