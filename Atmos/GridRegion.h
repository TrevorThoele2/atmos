#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "MaterialAsset.h"
#include "GridPosition.h"

namespace Atmos::Render
{
    class GridRegion final : public Arca::ClosedTypedRelic<GridRegion>
    {
    public:
        std::vector<Grid::Position> points;
        Grid::Position::Value z;
        Arca::Index<Asset::Material> material;
    public:
        GridRegion(Init init);
        GridRegion(
            Init init,
            const std::vector<Grid::Position>& points,
            Grid::Position::Value z,
            Arca::Index<Asset::Material> material);
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::GridRegion>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Render::GridRegion";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::GridRegion, BinaryArchive> final :
        public ArcaCompositeScribe<Atmos::Render::GridRegion, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}