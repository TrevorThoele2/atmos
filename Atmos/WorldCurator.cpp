#include "WorldCurator.h"

#include "DataAlgorithms.h"

namespace Atmos::World
{
    Curator::Curator(Init init, Manager& manager) : Arca::Curator(init), manager(&manager)
    {}

    void Curator::Handle(const RequestField& command)
    {
        manager->Request(command.id);
    }

    void Curator::Handle(const ModifyProperties& command)
    {
        auto properties = manager->CurrentWorldProperties();
        ApplyPropertyModifications(properties, command.add, command.remove, command.replace);
        manager->SetWorldProperties(properties);
    }

    std::vector<Property> Curator::Handle(const RetrieveProperties& command)
    {
        return manager->CurrentWorldProperties();
    }
}