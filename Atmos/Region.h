#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "MaterialAsset.h"
#include "Position2D.h"

namespace Atmos::Render
{
    class Region final : public Arca::ClosedTypedRelic<Region>
    {
    public:
        std::vector<Position2D> points;
        Position2D::Value z;
        Arca::Index<Asset::Material> material;
    public:
        Region(Init init);
        Region(
            Init init,
            const std::vector<Position2D>& points,
            Position2D::Value z,
            Arca::Index<Asset::Material> material);
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::Region>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Render::Region";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::Region, BinaryArchive> final :
        public ArcaCompositeScribe<Atmos::Render::Region, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}