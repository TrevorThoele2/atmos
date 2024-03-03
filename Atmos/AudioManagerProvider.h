#pragma once

#include "UniqueProvider.h"
#include "AudioManager.h"

namespace Atmos::Audio
{
    class AudioManagerProvider final
        : public Arca::ClosedTypedRelicAutomation<AudioManagerProvider>
        , public UniqueProvider<AudioManager>
    {
    public:
        AudioManagerProvider();
        AudioManagerProvider(const AudioManagerProvider& arg) = delete;
        AudioManagerProvider(AudioManagerProvider&& arg) = default;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::AudioManagerProvider>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Audio::AudioManagerProvider, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Audio::AudioManagerProvider, BinaryArchive>
    {};
}