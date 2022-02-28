#pragma once

#include "ArcaRelicIncludes.h"

#include "RenderCore.h"
#include "TextCore.h"
#include "Bounds.h"

#include "Point3D.h"
#include "Angle2D.h"
#include "Scalers2D.h"

namespace Atmos::Render
{
    class Text
    {
    public:
        using Index = int;
        using BoundsT = Spatial::Bounds;
    public:
        [[nodiscard]] String String() const;

        [[nodiscard]] Arca::Index<Asset::Font> Font() const;
        [[nodiscard]] Material Material() const;

        [[nodiscard]] float WrapWidth() const;
        [[nodiscard]] bool Bold() const;
        [[nodiscard]] bool Italics() const;

        [[nodiscard]] Color Color() const;

        [[nodiscard]] Spatial::Point3D Position() const;
        [[nodiscard]] Spatial::Scalers2D Scalers() const;
        [[nodiscard]] Spatial::Size2D Size() const;
        [[nodiscard]] Spatial::Angle2D Rotation() const;
    public:
        bool operator==(const Text& arg) const;
        bool operator!=(const Text& arg) const;
    protected:
        Text(
            Arca::RelicInit init,
            const Atmos::String& string,
            Arca::Index<Asset::Font> font,
            const Render::Material& material,
            float wrapWidth,
            bool bold,
            bool italics,
            const Render::Color& color,
            const Spatial::Point3D& position,
            const Spatial::Scalers2D& scalers,
            const Spatial::Angle2D& rotation,
            Spatial::Space boundsSpace);
        Text(Arca::RelicInit init, Arca::Serialization);
    private:
        Arca::RelicInit init;
    private:
        Arca::Index<RenderCore> renderCore;
        Arca::Index<TextCore> core;
        Arca::Index<BoundsT> bounds;
    };
}