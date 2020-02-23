#include "CurrentFieldCurator.h"

#include "CurrentFieldData.h"

namespace Atmos::World
{
    CurrentFieldCurator::CurrentFieldCurator(Init init) :
        Curator(init),
        debugFieldID(
            [this](Debug::Statistics& statistics)
            {
                statistics.game.fieldID = MutablePointer<CurrentFieldData>()->fieldID;
            },
            init.owner)
    {}

    void CurrentFieldCurator::Work()
    {

    }
}