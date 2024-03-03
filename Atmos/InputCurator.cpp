#include "InputCurator.h"

namespace Atmos::Input
{
    void Curator::InitializeImplementation()
    {
        managerProvider = Owner().Find<ManagerProvider>();

        mappedInputs = Owner().Find<MappedInputs>();
        mappedActions = Owner().Find<MappedActions>();
    }

    void Curator::WorkImplementation(Stage& stage)
    {
        
    }
}

namespace Arca
{
    const TypeName Traits<Atmos::Input::Curator>::typeName = "InputCurator";
}