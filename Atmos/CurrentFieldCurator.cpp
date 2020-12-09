#include "CurrentFieldCurator.h"

#include "CurrentFieldData.h"

namespace Atmos::World
{
    CurrentFieldCurator::CurrentFieldCurator(Init init) :
        Curator(init),
        debugFieldID(
            [this](Debug::Statistics& statistics)
            {
                statistics.game.fieldID = MutablePointer().Of<CurrentFieldData>()->fieldID;
            },
            MutablePointer())
    {}

    void CurrentFieldCurator::Handle(const Work&)
    {

    }
}