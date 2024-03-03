#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"

#include "Sound.h"

namespace Atmos
{
    class SoundSystem : public ObjectSystem
    {
    public:
        SoundSystem(ObjectManager& objectManager);

        void ResumeAll();
        void PauseAll();
    private:
        typedef ObjectBatch<nSound> SoundBatch;
        SoundBatch sounds;
    private:
        void OnCameraMoved();
    };
}