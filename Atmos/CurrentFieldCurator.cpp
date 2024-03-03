#include "CurrentFieldCurator.h"

#include "CurrentFieldData.h"
#include "DebugStatistics.h"

namespace Atmos::World
{
    void CurrentFieldCurator::Work()
    {
        Data<Debug::Statistics>()->game.fieldID = Data<CurrentFieldData>()->fieldID;
    }
}