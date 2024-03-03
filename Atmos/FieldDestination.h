#pragma once

#include "FieldID.h"
#include "GridPoint.h"
#include "Angle2D.h"

#include "Serialization.h"

namespace Atmos::World
{
    struct FieldDestination
    {
        Spatial::Angle2D direction = 0.0f;
        Spatial::Grid::Point position;
        FieldID id = 0;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::World::FieldDestination> final
    {
    public:
        using ObjectT = Atmos::World::FieldDestination;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("direction", object.direction);
            format("position", object.position);
            format("id", object.id);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::World::FieldDestination, Format> final
    {
        using Category = CompositeScribeCategory<Atmos::World::FieldDestination>;
    };
}