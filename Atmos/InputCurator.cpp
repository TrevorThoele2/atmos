#include "InputCurator.h"

namespace Atmos::Input
{
    void Curator::InitializeImplementation()
    {
        manager = Arca::ComputedPtr<Manager*>(Owner());

        mappedInputs = Arca::GlobalPtr<MappedInputs>(Owner());
        mappedActions = Arca::GlobalPtr<MappedActions>(Owner());
    }

    void Curator::WorkImplementation(Stage& stage)
    {
        
    }
}