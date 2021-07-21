#include "WorldCurator.h"

#include "Map.h"
#include "FieldChanging.h"

#include "DataAlgorithms.h"

namespace Atmos::World
{
    Curator::Curator(Init init, Manager& manager) : Arca::Curator(init), manager(&manager)
    {}

    void Curator::Handle(const Work&)
    {
        if (manager->WillLockIn())
        {
            Owner().Raise(FieldChanging{});
            manager->LockIn();
        }
    }

    void Curator::Handle(const RequestField& command)
    {
        manager->Request(command.id);
    }

    void Curator::Handle(const ModifyEntityBoundary& command)
    {
        auto& entityBoundary = MutablePointer().Of<Map>()->entityBoundary;
        ApplyAddRemoveModifications(entityBoundary, command.add, command.remove);
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