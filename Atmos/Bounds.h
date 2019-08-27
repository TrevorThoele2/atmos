#pragma once

#include <Arca/ShardTraits.h>
#include <Arca/Reliquary.h>
#include <Arca/Serialization.h>

#include "Position3D.h"
#include "Size3D.h"
#include "Scalers3D.h"
#include "AxisAlignedBox3D.h"

namespace Atmos
{
    class Bounds
    {
    public:
        void Position(const Position3D& to);
        void Size(const Size3D& to);

        [[nodiscard]] Position3D Position() const;
        [[nodiscard]] Size3D Size() const;
        [[nodiscard]] AxisAlignedBox3D Box() const;
    private:
        Position3D position;
        Size3D size;
        AxisAlignedBox3D box;
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
        static const TypeName typeName;
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