#pragma once

#include "Image.h"

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
            Arca::Index<Asset::Material> material,
            const Render::Color& color,
            const Spatial::Point3D& position,
            const Spatial::Scalers2D& scalers,
            const Spatial::Angle2D& rotation);
        DynamicImage(Arca::RelicInit init, Arca::Serialization serialization);
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Render::DynamicImage>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Render::DynamicImage";
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
        template<class Archive>
        void Scriven(ObjectT&, Archive&)
        {}
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Render::DynamicImage, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::DynamicImage>;
    };
}