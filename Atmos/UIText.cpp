#include "UIText.h"

namespace Atmos::UI
{
    Text::Text(
        Arca::RelicInit init,
        const Atmos::String& string,
        Arca::Index<Asset::Font> asset,
        const Render::Material& material,
        float wrapWidth,
        bool bold,
        bool italics,
        const Render::Color& color,
        const Spatial::Point3D& position,
        const Spatial::Scalers2D& scalers,
        const Spatial::Angle2D& rotation)
        :
        Render::Text(
            init,
            string,
            asset,
            material,
            wrapWidth,
            bold,
            italics,
            color,
            position,
            scalers,
            rotation,
            Spatial::Space::Screen)
    {}

    Text::Text(Arca::RelicInit init, Arca::Serialization serialization) :
        Render::Text(init, serialization)
    {}

    bool Text::operator==(const Text& arg) const
    {
        return Render::Text::operator==(arg);
    }

    bool Text::operator!=(const Text& arg) const
    {
        return !(*this == arg);
    }
}