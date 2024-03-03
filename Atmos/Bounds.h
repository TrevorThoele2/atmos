#pragma once

#include <Arca/Shard.h>
#include <Arca/Reliquary.h>
#include <Arca/Serialization.h>

#include "Position3D.h"
#include "Size2D.h"
#include "Scalers2D.h"
#include "AxisAlignedBox2D.h"

namespace Atmos
{
    class Bounds
    {
    public:
        Bounds();
        Bounds(const Position3D& position, const Size2D& baseSize, const Scalers2D& scalers);

        void Position(const Position3D& to);
        void BaseSize(const Size2D& to);
        void Scalers(const Scalers2D& to);

        [[nodiscard]] Position3D Position() const;
        [[nodiscard]] Size2D BaseSize() const;
        [[nodiscard]] Size2D Size() const;
        [[nodiscard]] Scalers2D Scalers() const;
        [[nodiscard]] AxisAlignedBox2D Box() const;
    private:
        Position3D position;
        Size2D baseSize;
        Scalers2D scalers;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Bounds>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static inline const TypeName typeName = "Bounds";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Bounds, BinaryArchive> final
        : public ArcaCompositeScribe<Atmos::Bounds, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}