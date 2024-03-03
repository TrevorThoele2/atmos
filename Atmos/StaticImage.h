#pragma once

#include "Image.h"

#include "Bounds.h"

namespace Atmos::Render
{
    class StaticImage final : public Image<false>
    {
    public:
        StaticImage(
            Arca::RelicInit init,
            Arca::Index<Asset::Image> asset,
            ImageCore::Index assetIndex,
            Arca::Index<Asset::Material> material,
            const Render::Color& color,
            const Spatial::Point3D& position,
            const Spatial::Scalers2D& scalers,
            const Spatial::Angle2D& rotation);
        StaticImage(Arca::RelicInit init, Arca::Serialization serialization);

        bool operator==(const StaticImage& arg) const;
        bool operator!=(const StaticImage& arg) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::StaticImage>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Render::StaticImage";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::StaticImage> final
    {
    public:
        using ObjectT = Atmos::Render::StaticImage;
    public:
        template<class Format>
        void Scriven(ObjectT&, Format&)
        {}
    };

    template<class Format>
    struct ScribeTraits<Atmos::Render::StaticImage, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::StaticImage>;
    };
}