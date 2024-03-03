#pragma once

#include "Image.h"

namespace Atmos::Render
{
    class RelativeImage final : public Image<RelativeImage, false>
    {
    public:
        RelativeImage(
            Init init,
            Arca::Index<Asset::Image> asset,
            ImageCore::Index assetIndex,
            Arca::Index<Asset::Material> material,
            const Render::Color& color,
            const Spatial::Point3D& position,
            const Spatial::Scalers2D& scalers,
            const Spatial::Angle& rotation);
        RelativeImage(Init init, Arca::Serialization serialization);
    private:

    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::RelativeImage>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Render::RelativeImage";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::RelativeImage, BinaryArchive> final
        : public ArcaCompositeScribe<Atmos::Render::RelativeImage, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override {}
    };
}