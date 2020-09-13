#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "Bounds.h"
#include "Angle3D.h"
#include "AxisAlignedBox2D.h"
#include "ScreenSize.h"

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

        [[nodiscard]] Spatial::AxisAlignedBox2D Sides() const;
    public:
        explicit Camera(Init init, Spatial::ScreenSize screenSize);
        explicit Camera(Init init, Arca::Serialization);
    private:
        Arca::Index<Spatial::Bounds> bounds;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::Camera>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Render::Camera";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::Camera> final
    {
    public:
        using ObjectT = Atmos::Render::Camera;
    public:
        template<class Archive>
        void Scriven(ObjectT&, Archive&)
        {}
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Render::Camera, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::Camera>;
    };
}