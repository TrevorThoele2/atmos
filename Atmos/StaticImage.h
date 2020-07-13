#pragma once

#include "Image.h"

namespace Atmos::Render
{
    class StaticImage final : public Image<StaticImage, false>
    {
    public:
        StaticImage(
            Init init,
            Arca::Index<Asset::Image> asset,
            ImageCore::Index assetIndex,
            Arca::Index<Asset::Material> material,
            const Render::Color& color,
            const Spatial::Point3D& position,
            const Spatial::Scalers2D& scalers,
            const Spatial::Angle& rotation);
        StaticImage(Init init, Arca::Serialization serialization);
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::StaticImage>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Render::StaticImage";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::StaticImage, BinaryArchive> final
        : public ArcaCompositeScribe<Atmos::Render::StaticImage, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override {}
    };
}