#include "InputGlobalArchiveInterface.h"

#include <Arca/ReliquaryOrigin.h>
#include "TypeRegistration.h"

namespace Atmos::World::Serialization
{
    InputGlobalArchiveInterface::InputGlobalArchiveInterface(const File::Path& filePath) :
        ArchiveInterface(filePath),
        archive(filePath, "ATMOS GLOBAL")
    {
        Load();
    }

    std::unique_ptr<Arca::Reliquary> InputGlobalArchiveInterface::TakeGlobalReliquary()
    {
        return std::move(globalReliquary);
    }

    ::Inscription::BinaryArchive& InputGlobalArchiveInterface::Archive()
    {
        return archive;
    }

    const ::Inscription::BinaryArchive& InputGlobalArchiveInterface::Archive() const
    {
        return archive;
    }

    void InputGlobalArchiveInterface::Load()
    {
        auto reliquaryOrigin = Arca::ReliquaryOrigin();
        RegisterTypes(reliquaryOrigin);

        globalReliquary = reliquaryOrigin.Actualize();

        archive(*globalReliquary);
    }
}