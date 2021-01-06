#pragma once

#include <Arca/Relic.h>

#include "Bounds.h"
#include "Angle3D.h"
#include "AxisAlignedBox2D.h"
#include "ScreenSize.h"

namespace Atmos::Render
{
    class Camera final
    {
    public:
        [[nodiscard]] Spatial::Point3D Position() const;
        [[nodiscard]] Spatial::Size2D Size() const;

        [[nodiscard]] Spatial::AxisAlignedBox2D Sides() const;
    public:
        explicit Camera(Arca::RelicInit init, Spatial::ScreenSize screenSize);
        explicit Camera(Arca::RelicInit init, Arca::Serialization);
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
        static TypeName TypeName() { return "Atmos::Render::Camera"; }
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