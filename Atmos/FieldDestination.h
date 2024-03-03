#pragma once

#include "FieldID.h"
#include "GridPoint.h"
#include "Angle2D.h"

#include "Serialization.h"

namespace Atmos::World
{
    struct FieldDestination
    {
        Spatial::Angle2D direction;
        Spatial::Grid::Point position;
        FieldID id;
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
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("direction", object.direction);
            archive("position", object.position);
            archive("id", object.id);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::World::FieldDestination, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::World::FieldDestination>;
    };
}