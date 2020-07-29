#pragma once

#include "Image.h"

#include "Angle2D.h"
#include "Angle3D.h"

namespace Atmos::Render
{
    class DynamicImage final : public Image<DynamicImage, true>
    {
    public:
        void MoveTo(const Spatial::Point3D& to) const;
        void MoveBy(const Spatial::Point3D& by) const;
        void MoveDirection(const Spatial::Angle3D& direction, Spatial::Point3D::Value amount) const;
        void ScaleTo(const Spatial::Scalers2D& to) const;
        void RotateTo(const Spatial::Angle2D& to) const;
    public:
        DynamicImage(
            Init init,
            Arca::Index<Asset::Image> asset,
            ImageCore::Index assetIndex,
            Arca::Index<Asset::Material> material,
            const Render::Color& color,
            const Spatial::Point3D& position,
            const Spatial::Scalers2D& scalers,
            const Spatial::Angle2D& rotation);
        DynamicImage(Init init, Arca::Serialization serialization);
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