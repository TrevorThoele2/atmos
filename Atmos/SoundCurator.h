#pragma once

#include <Arca/Curator.h>
#include <Arca/Batch.h>

#include "Sound.h"

namespace Atmos::Audio
{
    class SoundCurator final : public Arca::Curator
    {
    public:
        explicit SoundCurator(Init init);
    public:
        void ResumeAll();
        void PauseAll();
    private:
        using Batch = ::Arca::Batch<Sound>;
        Batch batch;
    private:
        void OnCameraMoved();
        void OnEngineFocusLost();
        void OnEngineFocusRegained();
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::SoundCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "SourceCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Audio::SoundCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Audio::SoundCurator, BinaryArchive>
    {};
}