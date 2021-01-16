#pragma once

#include "Volume.h"
#include "Point2D.h"

namespace Atmos::Audio::Resource
{
    class Sound
    {
    public:
        virtual ~Sound() = 0;
    public:
        virtual void Restart() = 0;
        virtual void Pause() = 0;
        virtual void Resume() = 0;
        virtual void SetVolume(Volume volume) = 0;
        virtual void SetPosition(const Spatial::Point2D& position) = 0;
    protected:
        static const Spatial::Point2D::Value maxDistance;

        Sound() = default;
    };
}