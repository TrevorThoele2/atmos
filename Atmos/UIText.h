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
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::UI::Text"; }
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
        template<class Archive>
        void Scriven(ObjectT&, Archive&)
        {}
    };

    template<class Archive>
    struct ScribeTraits<Atmos::UI::Text, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::UI::Text>;
    };
}