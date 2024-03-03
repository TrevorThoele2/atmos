#pragma once

#include <Arca/Shard.h>
#include <Arca/Reliquary.h>
#include <Arca/Serialization.h>

#include "Point3D.h"
#include "Size2D.h"
#include "Scalers2D.h"
#include "Angle.h"

namespace Atmos::Spatial
{
    class Bounds
    {
    public:
        Bounds();
        Bounds(const Point3D& position, const Size2D& baseSize, const Scalers2D& scalers, const Angle& rotation);

        void Position(const Point3D& to);
        void BaseSize(const Size2D& to);
        void Scalers(const Scalers2D& to);
        void Rotation(const Angle& to);

        [[nodiscard]] Point3D Position() const;
        [[nodiscard]] Size2D BaseSize() const;
        [[nodiscard]] Size2D Size() const;
        [[nodiscard]] Scalers2D Scalers() const;
        [[nodiscard]] Angle Rotation() const;
    private:
        Point3D position;
        Size2D baseSize;
        Scalers2D scalers;
        Angle rotation;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::Bounds>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static inline const TypeName typeName = "Atmos::Spatial::Bounds";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::Bounds, BinaryArchive> final
        : public ArcaCompositeScribe<Atmos::Spatial::Bounds, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}