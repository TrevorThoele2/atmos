#pragma once

#include "Image.h"

#include "ImageCore.h"
#include "Bounds.h"

namespace Atmos::Render
{
    class DynamicImage final : public Image<DynamicImage, true>
    {
    public:
        void Position(const Position3D& to) const;
        void Scalers(const Scalers2D& to) const;
        void Rotation(const Angle& to) const;
    public:
        DynamicImage(Init init);
        DynamicImage(
            Init init,
            Arca::Index<Asset::Image> asset,
            ImageCore::Index assetIndex,
            Arca::Index<Asset::Material> material,
            const Render::Color& color,
            const Position3D& position,
            const Scalers2D& scalers,
            const Angle& rotation);
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
    class Scribe<::Atmos::Render::DynamicImage, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Render::DynamicImage, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override {}
    };
}