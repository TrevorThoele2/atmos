#pragma once

#include <Arca/Shard.h>
#include <Arca/Reliquary.h>
#include <Arca/Serialization.h>

#include "Position3D.h"
#include "Size2D.h"
#include "Scalers3D.h"
#include "AxisAlignedBox2D.h"

namespace Atmos
{
    class Bounds
    {
    public:
        void Position(const Position3D& to);
        void Size(const Size2D& to);

        [[nodiscard]] Position3D Position() const;
        [[nodiscard]] Size2D Size() const;
        [[nodiscard]] AxisAlignedBox2D Box() const;
    private:
        Position3D position;
        Size2D size;
        AxisAlignedBox2D box;
    private:
        void CalculateBox();
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