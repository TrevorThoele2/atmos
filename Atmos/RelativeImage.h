#pragma once

#include "Image.h"

#include "RelativeBounds.h"

namespace Atmos::Render
{
    class RelativeImage final : public Image<RelativeImage, true>
    {
    public:
        RelativeImage(
            Init init,
            Arca::Index<Asset::Image> asset,
            ImageCore::Index assetIndex,
            Arca::Index<Asset::Material> material,
            const Render::Color& color,
            const Spatial::Point3D& delta,
            const Spatial::Scalers2D& scalers,
            const Spatial::Angle2D& rotation);
        RelativeImage(Init init, Arca::Serialization serialization);
    private:
        Arca::Index<Spatial::RelativeBounds> relativeBounds;
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