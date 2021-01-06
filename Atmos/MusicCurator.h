#pragma once

#include <Arca/Curator.h>

#include "FilePath.h"

#include "Serialization.h"

namespace Atmos::Audio
{
    class MusicCurator final : public ::Arca::Curator
    {
    public:
        MusicCurator() = default;

        void BeginPlaying(const File::Path& fileName);
        void TerminateCurrent();

        void ResumeCurrent();
        void PauseCurrent();
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::MusicCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Audio::MusicCurator"; }
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Audio::MusicCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Audio::MusicCurator>;
    };
}