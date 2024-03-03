#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "LineWidth.h"

#include "Position2D.h"
#include "Color.h"

namespace Atmos::Render
{
    class Line : public Arca::ClosedTypedRelic<Line>
    {
    public:
        Position2D from;
        Position2D to;
        Position2D::Value z = 0;

        LineWidth width = 0;

        Color color;
    public:
        explicit Line(Init init) : ClosedTypedRelic(init)
        {}

        Line(Init init, Position2D from, Position2D to);
        Line(Init init, Position2D from, Position2D to, Position2D::Value z, LineWidth width, Color color);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::Line>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Line";
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