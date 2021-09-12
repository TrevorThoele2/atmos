#pragma once

#include "ArcaRelicIncludes.h"
#include "LineWidth.h"

#include "Point2D.h"
#include "Color.h"
#include "RenderCore.h"

#include <Inscription/VectorScribe.h>

namespace Atmos::Render
{
    class Line
    {
    public:
        Arca::Index<RenderCore> renderCore;

        std::vector<Spatial::Point2D> points;
        Spatial::Point2D::Value z = 0;
        LineWidth width = 0;
    public:
        Line(Arca::RelicInit init);
        Line(Arca::RelicInit init, const std::vector<Spatial::Point2D>& points, Arca::Index<Asset::Material> material);
        Line(
            Arca::RelicInit init,
            const std::vector<Spatial::Point2D>& points,
            Spatial::Point2D::Value z,
            Arca::Index<Asset::Material> material,
            LineWidth width,
            Color color);
        Line(Arca::RelicInit init, Arca::Serialization);

        bool operator==(const Line& arg) const;
        bool operator!=(const Line& arg) const;
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
            archive("renderCore", object.renderCore);
            archive("points", object.points);
            archive("z", object.z);
            archive("width", object.width);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Render::Line, Archive>
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::Line>;
    };
}