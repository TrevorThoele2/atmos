#include "InputCurator.h"

namespace Atmos::Input
{
    Curator::Curator(Init init) :
        Arca::Curator(init), manager(init.owner), mappedInputs(init.owner), mappedActions(init.owner)
    {}

    void Curator::Work()
    {
        
    }
}