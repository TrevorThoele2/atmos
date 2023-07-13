#pragma once

#include "Image.h"

#include "Bounds.h"

#include "Angle2D.h"
#include "Angle3D.h"

namespace Atmos::Render
{
    class DynamicImage final : public Image<true>
    {
    public:
        DynamicImage(
            Arca::RelicInit init,
            Arca::Index<Asset::Image> asset,
            ImageCore::Index assetIndex,
            const Arca::Index<Asset::Material>& material,
            const Render::Color& color,
            const Spatial::Point3D& position,
            const Spatial::Scalers2D& scalers,
            const Spatial::Angle2D& rotation);
        DynamicImage(Arca::RelicInit init, Arca::Serialization serialization);

        bool operator==(const DynamicImage& arg) const;
        bool operator!=(const DynamicImage& arg) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Render::DynamicImage>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Render::DynamicImage";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::DynamicImage> final
    {
    public:
        using ObjectT = Atmos::Render::DynamicImage;
    public:
        template<class Format>
        void Scriven(ObjectT&, Format&)
        {}
    };

    template<class Format>
    struct ScribeTraits<Atmos::Render::DynamicImage, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::DynamicImage>;
    };
}