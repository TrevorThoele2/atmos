#pragma once

#include "ObjectSystem.h"

#include "FilePath.h"

namespace Atmos
{
    class MusicSystem : public ObjectSystem
    {
    public:
        MusicSystem(ObjectManager& manager);
        MusicSystem(const ::Inscription::Table<MusicSystem>& table);

        void BeginPlaying(const FileName& fileName);
        void TerminateCurrent();

        void ResumeCurrent();
        void PauseCurrent();
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::MusicSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}