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
        static inline const TypeName typeName = "MusicCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Audio::MusicCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Audio::MusicCurator, BinaryArchive>
    {};
}