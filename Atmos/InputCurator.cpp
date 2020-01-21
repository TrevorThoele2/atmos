#include "InputCurator.h"

namespace Atmos::Input
{
    void Curator::InitializeImplementation()
    {
        manager = Arca::ComputedIndex<Manager*>(Owner());

        mappedInputs = Arca::GlobalIndex<MappedInputs>(Owner());
        mappedActions = Arca::GlobalIndex<MappedActions>(Owner());
    }

    void Curator::WorkImplementation(Stage& stage)
    {
        
    }
}