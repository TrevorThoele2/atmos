#pragma once

#include "Image.h"

#include "ImageCore.h"
#include "Bounds.h"

namespace Atmos::Render
{
    class StaticImage final : public Image<StaticImage, false>
    {
    public:
        explicit StaticImage(Init init);
        StaticImage(
            Init init,
            Arca::Index<Asset::Image> asset,
            ImageCore::Index assetIndex,
            Arca::Index<Asset::Material> material,
            const Render::Color& color,
            const Position3D& position,
            const Scalers2D& scalers);
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
        static inline const TypeName typeName = "StaticImage";
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