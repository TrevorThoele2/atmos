#pragma once

#include "Point2D.h"
#include "Size2D.h"

#include "Serialization.h"

namespace Atmos::Spatial
{
    struct AxisAlignedBox2D
    {
    public:
        using Coordinate = float;

        Point2D center;
        Size2D size;
    public:
        AxisAlignedBox2D() = default;
        AxisAlignedBox2D(const Point2D& center, const Size2D& size);

        bool operator==(const AxisAlignedBox2D& arg) const;
        bool operator!=(const AxisAlignedBox2D& arg) const;

        [[nodiscard]] Coordinate Left() const;
        [[nodiscard]] Coordinate Top() const;
        [[nodiscard]] Coordinate Right() const;
        [[nodiscard]] Coordinate Bottom() const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::AxisAlignedBox2D> final
    {
    public:
        using ObjectT = Atmos::Spatial::AxisAlignedBox2D;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("center", object.center);
            archive("size", object.size);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Spatial::AxisAlignedBox2D, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::AxisAlignedBox2D>;
    };
}