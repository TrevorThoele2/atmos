#pragma once

#include <Arca/Relic.h>
#include "LineWidth.h"

#include "Point2D.h"
#include "Color.h"
#include "LineMaterialAsset.h"

#include <Inscription/VectorScribe.h>

namespace Atmos::Render
{
    class Line
    {
    public:
        std::vector<Spatial::Point2D> points;
        Spatial::Point2D::Value z = 0;
        Arca::Index<Asset::LineMaterial> material;

        LineWidth width = 0;

        Color color;
    public:
        Line() = default;
        Line(const std::vector<Spatial::Point2D>& points, Arca::Index<Asset::LineMaterial> material);
        Line(
            const std::vector<Spatial::Point2D>& points,
            Spatial::Point2D::Value z,
            Arca::Index<Asset::LineMaterial> material,
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
        static TypeName TypeName() { return "Atmos::Render::Line"; }
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::Line> final
    {
    public:
        using ObjectT = Atmos::Render::Line;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("points", object.points);
            archive("z", object.z);
            archive("material", object.material);
            archive("width", object.width);
            archive("color", object.color);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Render::Line, Archive>
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::Line>;
    };
}