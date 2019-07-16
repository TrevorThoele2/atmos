#pragma once

#include "ObjectSystem.h"

#include "FileName.h"

namespace Atmos
{
    class MusicSystem : public ObjectSystem
    {
    public:
        MusicSystem(ObjectManager& manager);

        void BeginPlaying(const FileName& fileName);
        void TerminateCurrent();

        void ResumeCurrent();
        void PauseCurrent();
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::MusicSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::MusicSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}