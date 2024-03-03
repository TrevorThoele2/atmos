#include "OutputWorldArchiveInterface.h"

#include <Inscription/OutputJumpTable.h>

namespace Atmos::World::Serialization
{
    OutputWorldArchiveInterface::OutputWorldArchiveInterface
    (
        const File::Path& filePath,
        std::vector<Field>& fields
    ) :
        ArchiveInterface(filePath),
        archive(filePath, "ATMOS WORLD", CurrentVersion())
    {
        Save(fields);
    }

    ::Inscription::Version OutputWorldArchiveInterface::CurrentVersion()
    {
        return 1;
    }

    ::Inscription::BinaryArchive& OutputWorldArchiveInterface::Archive()
    {
        return archive;
    }

    const ::Inscription::BinaryArchive& OutputWorldArchiveInterface::Archive() const
    {
        return archive;
    }

    void OutputWorldArchiveInterface::Save(std::vector<Field>& fields)
    {
        ::Inscription::OutputJumpTable<FieldID, Field> jumpTable;
        for (auto& field : fields)
            jumpTable.Add(field.id, field);
        archive(jumpTable);
    }
}