#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "LineWidth.h"

#include "Position2D.h"
#include "Color.h"
#include "MaterialAsset.h"

namespace Atmos::Render
{
    class Line : public Arca::ClosedTypedRelic<Line>
    {
    public:
        std::vector<Position2D> points;
        Position2D::Value z = 0;
        Arca::Index<Asset::Material> material;

        LineWidth width = 0;

        Color color;
    public:
        explicit Line(Init init) : ClosedTypedRelic(init)
        {}

        Line(Init init, const std::vector<Position2D>& points, Arca::Index<Asset::Material> material);
        Line(Init init,
            const std::vector<Position2D>& points,
            Position2D::Value z,
            Arca::Index<Asset::Material> material,
            LineWidth width,
            Color color);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::Line>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Render::Line";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::Line, BinaryArchive> final :
        public ArcaCompositeScribe<Atmos::Render::Line, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}