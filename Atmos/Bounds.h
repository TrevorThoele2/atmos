#pragma once

#include "ArcaShardIncludes.h"

#include "Space.h"
#include "Point3D.h"
#include "Size2D.h"
#include "Scalers2D.h"
#include "Angle2D.h"

namespace Atmos::Spatial
{
    class Bounds
    {
    public:
        Bounds();
        Bounds(Space space, const Point3D& position, const Size2D& baseSize, const Scalers2D& scalers, const Angle2D& rotation);

        bool operator==(const Bounds& arg) const;
        bool operator!=(const Bounds& arg) const;

        void Position(const Point3D& to);
        void BaseSize(const Size2D& to);
        void Scalers(const Scalers2D& to);
        void Rotation(const Angle2D& to);

        [[nodiscard]] Point3D Position() const;
        [[nodiscard]] Size2D BaseSize() const;
        [[nodiscard]] Size2D Size() const;
        [[nodiscard]] Scalers2D Scalers() const;
        [[nodiscard]] Angle2D Rotation() const;
        [[nodiscard]] Space Space() const;
    private:
        Spatial::Space space = Space::World;
        Point3D position;
        Size2D baseSize;
        Scalers2D scalers;
        Angle2D rotation = 0.0f;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::Bounds>
    {
        static constexpr ObjectType objectType = ObjectType::Shard;
        static const inline TypeName typeName = "Atmos::Spatial::Bounds";
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Spatial::Space, Format>
    {
        using Category = EnumScribeCategory<Atmos::Spatial::Space>;
    };

    template<>
    class Scribe<Atmos::Spatial::Bounds> final
    {
    public:
        using ObjectT = Atmos::Spatial::Bounds;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("space", object.space);
            format("position", object.position);
            format("baseSize", object.baseSize);
            format("scalers", object.scalers);
            format("rotation", object.rotation);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Spatial::Bounds, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Spatial::Bounds>;
    };
}