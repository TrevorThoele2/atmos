#include "Text.h"

namespace Atmos::Render
{
    String Text::String() const
    {
        return core->string;
    }

    Arca::Index<Asset::Font> Text::Font() const
    {
        return core->font;
    }

    Arca::Index<Asset::Material> Text::Material() const
    {
        return renderCore->material;
    }

    float Text::WrapWidth() const
    {
        return core->wrapWidth;
    }

    bool Text::Bold() const
    {
        return core->bold;
    }

    bool Text::Italics() const
    {
        return core->italics;
    }

    Render::Color Text::Color() const
    {
        return renderCore->color;
    }

    Spatial::Point3D Text::Position() const
    {
        return bounds->Position();
    }

    Spatial::Size2D Text::Size() const
    {
        return bounds->Size();
    }

    Spatial::Angle2D Text::Rotation() const
    {
        return bounds->Rotation();
    }
    
    Text::Text(
        Arca::RelicInit init,
        const Atmos::String& string,
        Arca::Index<Asset::Font> font,
        Arca::Index<Asset::Material> material,
        float wrapWidth,
        bool bold,
        bool italics,
        const Render::Color& color,
        const Spatial::Point3D& position,
        const Spatial::Scalers2D& scalers,
        const Spatial::Angle2D& rotation,
        Spatial::Space boundsSpace)
        :
        init(init)
    {
        renderCore = init.Create<RenderCore>(material, color);
        core = init.Create<TextCore>(string, font, wrapWidth, bold, italics);
        const auto baseSize = Spatial::Size2D{ 0, 0 };
        bounds = init.Create<BoundsT>(boundsSpace, position, baseSize, scalers, rotation);
    }

    Text::Text(Arca::RelicInit init, Arca::Serialization) :
        init(init),
        renderCore(init.Find<RenderCore>()),
        core(init.Find<TextCore>()),
        bounds(init.Find<BoundsT>())
    {}
}