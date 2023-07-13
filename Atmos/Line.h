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
        Line(Arca::RelicInit init, const std::vector<Spatial::Point2D>& points, const Arca::Index<Asset::Material>& material);
        Line(
            Arca::RelicInit init,
            const std::vector<Spatial::Point2D>& points,
            Spatial::Point2D::Value z,
            const Arca::Index<Asset::Material>& material,
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
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Render::Line";
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
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("renderCore", object.renderCore);
            format("points", object.points);
            format("z", object.z);
            format("width", object.width);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Render::Line, Format>
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::Line>;
    };
}