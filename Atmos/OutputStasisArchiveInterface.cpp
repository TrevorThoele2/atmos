#include "OutputStasisArchiveInterface.h"

#include <Inscription/OutputJumpTable.h>

namespace Atmos::World::Serialization
{
    OutputStasisArchiveInterface::OutputStasisArchiveInterface
    (
        const File::Path& filePath,
        std::vector<Field> fields
    ) :
        ArchiveInterface(filePath),
        archive(filePath, "ATMOS STASIS", CurrentVersion())
    {
        Save(std::move(fields));
    }

    ::Inscription::Version OutputStasisArchiveInterface::CurrentVersion()
    {
        return 1;
    }

    ::Inscription::BinaryArchive& OutputStasisArchiveInterface::Archive()
    {
        return archive;
    }

    const ::Inscription::BinaryArchive& OutputStasisArchiveInterface::Archive() const
    {
        return archive;
    }

    void OutputStasisArchiveInterface::Save(std::vector<Field> fields)
    {
        ::Inscription::OutputJumpTable<FieldID, Field> jumpTable;
        for (auto& field : fields)
            jumpTable.Add(field.id, field);
        archive(jumpTable);
    }
}