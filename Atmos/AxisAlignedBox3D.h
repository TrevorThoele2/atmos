#pragma once

#include "Point3D.h"
#include "Size3D.h"
#include "Angle3D.h"

#include "Serialization.h"

namespace Atmos::Spatial
{
    class AxisAlignedBox3D
    {
    public:
        using Coordinate = float;

        Point3D center;
        Size3D size;
    public:
        AxisAlignedBox3D() = default;
        AxisAlignedBox3D(const Point3D& center, const Size3D& size);

        bool operator==(const AxisAlignedBox3D& arg) const;
        bool operator!=(const AxisAlignedBox3D& arg) const;

        [[nodiscard]] Coordinate Left() const;
        [[nodiscard]] Coordinate Top() const;
        [[nodiscard]] Coordinate FarZ() const;
        [[nodiscard]] Coordinate Right() const;
        [[nodiscard]] Coordinate Bottom() const;
        [[nodiscard]] Coordinate NearZ() const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Spatial::AxisAlignedBox3D, BinaryArchive> final :
        public CompositeScribe<::Atmos::Spatial::AxisAlignedBox3D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}