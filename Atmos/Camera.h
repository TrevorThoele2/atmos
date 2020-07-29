#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "Bounds.h"
#include "Angle3D.h"
#include "AxisAlignedBox2D.h"

namespace Atmos::Render
{
    class Camera final : public Arca::ClosedTypedRelic<Camera>
    {
    public:
        void MoveTo(const Spatial::Point3D& to) const;
        void MoveBy(const Spatial::Point3D& by) const;
        void MoveDirection(const Spatial::Angle3D& direction, Spatial::Point3D::Value amount) const;
        void ScaleTo(const Spatial::Scalers2D& to) const;

        [[nodiscard]] Spatial::Point3D Position() const;
        [[nodiscard]] Spatial::Size2D Size() const;

        [[nodiscard]] Spatial::AxisAlignedBox2D ScreenSides() const;
    public:
        explicit Camera(Init init);
        explicit Camera(Init init, Arca::Serialization);
    private:
        Arca::Index<Spatial::Bounds> bounds;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Render::Camera>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Render::Camera";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::Camera, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Render::Camera, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}