#pragma once

#include "Text.h"
#include "Bounds.h"

namespace Atmos::UI
{
    class Text final : public Render::Text
    {
    public:
        Text(
            Arca::RelicInit init,
            const Atmos::String& string,
            Arca::Index<Asset::Font> asset,
            Arca::Index<Asset::Material> material,
            float wrapWidth,
            bool bold,
            bool italics,
            const Render::Color& color,
            const Spatial::Point3D& position,
            const Spatial::Scalers2D& scalers,
            const Spatial::Angle2D& rotation);
        Text(Arca::RelicInit init, Arca::Serialization serialization);

        bool operator==(const Text& arg) const;
        bool operator!=(const Text& arg) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::UI::Text>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::UI::Text";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::UI::Text> final
    {
    public:
        using ObjectT = Atmos::UI::Text;
    public:
        template<class Format>
        void Scriven(ObjectT&, Format&)
        {}
    };

    template<class Format>
    struct ScribeTraits<Atmos::UI::Text, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::UI::Text>;
    };
}