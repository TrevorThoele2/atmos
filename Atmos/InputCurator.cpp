#include "InputCurator.h"

namespace Atmos::Input
{
    Curator::Curator(Init init, Manager& manager) :
        Arca::Curator(init),
        manager(&manager), mappedInputs(init.owner), mappedActions(init.owner)
    {}

    void Curator::Work()
    {
        
    }
}