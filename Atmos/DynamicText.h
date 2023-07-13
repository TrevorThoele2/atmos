#pragma once

#include "Text.h"
#include "Bounds.h"

namespace Atmos::Render
{
    class DynamicText final : public Text
    {
    public:
        DynamicText(
            Arca::RelicInit init,
            const Atmos::String& string,
            Arca::Index<Asset::Font> asset,
            const Arca::Index<Asset::Material>& material,
            float wrapWidth,
            bool bold,
            bool italics,
            const Render::Color& color,
            const Spatial::Point3D& position,
            const Spatial::Scalers2D& scalers,
            const Spatial::Angle2D& rotation);
        DynamicText(Arca::RelicInit init, Arca::Serialization serialization);
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::DynamicText>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Render::DynamicText";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::DynamicText> final
    {
    public:
        using ObjectT = Atmos::Render::DynamicText;
    public:
        template<class Format>
        void Scriven(ObjectT&, Format&)
        {}
    };

    template<class Format>
    struct ScribeTraits<Atmos::Render::DynamicText, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::DynamicText>;
    };
}