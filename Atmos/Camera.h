#pragma once

#include "ArcaRelicIncludes.h"

#include "Bounds.h"
#include "Angle3D.h"
#include "AxisAlignedBox2D.h"
#include "Size2D.h"

namespace Atmos::Render
{
    class Camera final
    {
    public:
        [[nodiscard]] Spatial::Point3D Position() const;
        [[nodiscard]] Spatial::Size2D Size() const;

        [[nodiscard]] Spatial::AxisAlignedBox2D Sides() const;
    public:
        explicit Camera(Arca::RelicInit init, Spatial::Size2D screenSize);
        explicit Camera(Arca::RelicInit init, Arca::Serialization);

        bool operator==(const Camera& arg) const;
        bool operator!=(const Camera& arg) const;
    private:
        Arca::Index<Spatial::Bounds> bounds;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::Camera>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Render::Camera";
        static constexpr Locality locality = Locality::Global;
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
        template<class Format>
        void Scriven(ObjectT&, Format&)
        {}
    };

    template<class Format>
    struct ScribeTraits<Atmos::Render::Camera, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::Camera>;
    };
}