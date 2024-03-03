#include "DataCurator.h"

#include "DataCore.h"
#include "DataAlgorithms.h"

#include <Arca/Reliquary.h>

namespace Atmos
{
    DataCurator::DataCurator(Init init) : Curator(init)
    {}

    void DataCurator::Handle(const ModifyProperties& command)
    {
        auto mutableCore = MutablePointer().Of<DataCore>(command.id);
        ApplyPropertyModifications(mutableCore->properties, command.add, command.remove, command.replace);
    }
}