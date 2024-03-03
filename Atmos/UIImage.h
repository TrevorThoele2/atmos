#pragma once

#include "Image.h"

#include "Angle2D.h"
#include "Angle3D.h"

namespace Atmos::UI
{
    class Image final : public Render::Image<true>
    {
    public:
        Image(
            Arca::RelicInit init,
            Arca::Index<Asset::Image> asset,
            Render::ImageCore::Index assetIndex,
            Arca::Index<Asset::Material> material,
            const Render::Color& color,
            const Spatial::Point3D& position,
            const Spatial::Scalers2D& scalers,
            const Spatial::Angle2D& rotation);
        Image(Arca::RelicInit init, Arca::Serialization serialization);

        bool operator==(const Image& arg) const;
        bool operator!=(const Image& arg) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::UI::Image>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::UI::Image";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::UI::Image> final
    {
    public:
        using ObjectT = Atmos::UI::Image;
    public:
        template<class Archive>
        void Scriven(ObjectT&, Archive&)
        {}
    };

    template<class Archive>
    struct ScribeTraits<Atmos::UI::Image, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::UI::Image>;
    };
}