#include "OutputGlobalArchiveInterface.h"

namespace Atmos::World::Serialization
{
    OutputGlobalArchiveInterface::OutputGlobalArchiveInterface
    (
        const File::Path& filePath,
        Arca::Reliquary& globalReliquary
    ) :
        ArchiveInterface(filePath),
        archive(filePath, "ATMOS GLOBAL", CurrentVersion())
    {
        Save(globalReliquary);
    }

    ::Inscription::Version OutputGlobalArchiveInterface::CurrentVersion()
    {
        return 1;
    }

    ::Inscription::BinaryArchive& OutputGlobalArchiveInterface::Archive()
    {
        return archive;
    }

    const ::Inscription::BinaryArchive& OutputGlobalArchiveInterface::Archive() const
    {
        return archive;
    }

    void OutputGlobalArchiveInterface::Save(Arca::Reliquary& globalReliquary)
    {
        archive(globalReliquary);
    }
}