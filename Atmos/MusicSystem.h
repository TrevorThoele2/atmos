#pragma once

#include "ObjectSystem.h"

#include "FilePath.h"

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