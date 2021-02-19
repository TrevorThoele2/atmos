#pragma once

#include "GridPoint.h"
#include "GridSize.h"

#include "Serialization.h"

namespace Atmos::Spatial::Grid
{
    struct AxisAlignedBox
    {
    public:
        using Coordinate = int;

        Point center;
        Size size;
    public:
        AxisAlignedBox() = default;
        AxisAlignedBox(const Point& center, const Size& size);

        bool operator==(const AxisAlignedBox& arg) const;
        bool operator!=(const AxisAlignedBox& arg) const;

        [[nodiscard]] Coordinate Left() const;
        [[nodiscard]] Coordinate Top() const;
        [[nodiscard]] Coordinate Right() const;
        [[nodiscard]] Coordinate Bottom() const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::Grid::AxisAlignedBox> final
    {
    public:
        using ObjectT = Atmos::Spatial::Grid::AxisAlignedBox;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("center", object.center);
            archive("size", object.size);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Spatial::Grid::AxisAlignedBox, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::Grid::AxisAlignedBox>;
    };
}