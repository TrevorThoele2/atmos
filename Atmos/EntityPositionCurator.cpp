#include "EntityPositionCurator.h"

namespace Atmos::Entity
{
    PositionCurator::PositionCurator(Init init) :
        Curator(init), positionedEntities(init.owner)
    {}

    void PositionCurator::Handle(const MoveEntity& command)
    {
        
    }

    void PositionCurator::Handle(const MoveEntityInstant& command)
    {
        
    }

    void PositionCurator::Handle(const MoveEntityDirection& command)
    {
        
    }

    void PositionCurator::Handle(const RotateEntity& command)
    {
        
    }
}