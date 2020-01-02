#include "EntityPositionCurator.h"

#include "MoveEntity.h"
#include "MoveEntityInstant.h"
#include "MoveEntityDirection.h"
#include "RotateEntity.h"

#include <Arca/Reliquary.h>
#include "DebugStatistics.h"

namespace Atmos::Entity
{
    void PositionCurator::InitializeImplementation()
    {
        Owner().ExecuteOn<MoveEntity>(
            [this](const MoveEntity& signal)
            {

            });

        Owner().ExecuteOn<MoveEntityInstant>(
            [this](const MoveEntityInstant& signal)
            {

            });

        Owner().ExecuteOn<MoveEntityDirection>(
            [this](const MoveEntityDirection& signal)
            {

            });

        Owner().ExecuteOn<RotateEntity>(
            [this](const RotateEntity& signal)
            {

            });

        positionedEntities = Arca::GlobalPtr<PositionedEntities>(Owner());

        debugStatistics = Arca::GlobalPtr<Debug::Statistics>(Owner());
    }
}