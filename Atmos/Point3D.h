#pragma once

#include "Serialization.h"

namespace Atmos::Spatial
{
    class AxisAlignedBox3D;

    struct Point3D
    {
        using Value = float;
        Value x = 0.0f;
        Value y = 0.0f;
        Value z = 0.0f;

        bool operator==(const Point3D& arg) const;
        bool operator!=(const Point3D& arg) const;

        [[nodiscard]] bool Contains(Value left, Value right, Value top, Value bottom, Value nearZ, Value farZ) const;
        [[nodiscard]] bool Contains(const AxisAlignedBox3D& box) const;

        static Point3D FromScreen(const Point3D& convert, const Point3D& topLeftScreen, Value z);
        static Value DistanceBetween(const Point3D& starting, const Point3D& destination);
        static Point3D CenterOf(const std::vector<Point3D>& container);
        static Point3D CenterOf
        (
            const Point3D& nearTopLeft,
            const Point3D& nearBottomRight,
            const Point3D& farTopLeft
        );
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::Point3D, BinaryArchive> final :
        public CompositeScribe<Atmos::Spatial::Point3D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}