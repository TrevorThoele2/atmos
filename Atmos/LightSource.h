
#pragma once

#include "Color.h"

#include "Optional.h"

namespace Atmos
{
    class Canvas;
    class Position3D;
    class LightSource
    {
    public:
        typedef float Intensity;
    private:
        Color color;
        Intensity intensity;

        virtual bool IsInBounds() const = 0;

        virtual void SetPositionImpl(const Position3D &set) = 0;
        virtual Optional<Position3D> GetPositionImpl() const = 0;
        virtual bool HasPositionImpl() const = 0;
    public:
        LightSource(const Color &color, Intensity intensity);
        virtual ~LightSource() = 0 {}

        void Draw(Canvas &lightmap);

        void SetIntensity(Intensity set);
        Intensity GetIntensity() const;

        void SetPosition(const Position3D &set);
        Optional<Position3D> GetPosition() const;
        bool HasPosition() const;
    };
}