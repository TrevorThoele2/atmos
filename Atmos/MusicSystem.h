#pragma once

#include "ObjectSystem.h"

#include "FileName.h"

namespace Atmos::Audio
{
    class MusicSystem : public ObjectSystem
    {
    public:
        MusicSystem(ObjectManager& manager);

        void BeginPlaying(const File::Name& fileName);
        void TerminateCurrent();

        void ResumeCurrent();
        void PauseCurrent();
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Audio::MusicSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Audio::MusicSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}