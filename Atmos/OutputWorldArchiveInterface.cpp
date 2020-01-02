#include "OutputWorldArchiveInterface.h"

#include <Inscription/OutputJumpTable.h>

namespace Atmos::World::Serialization
{
    OutputWorldArchiveInterface::OutputWorldArchiveInterface(const File::Path& filePath) :
        archive(filePath, "ATMOS WORLD", 1)
    {}

    void OutputWorldArchiveInterface::Save(std::vector<Field>& fields)
    {
        ::Inscription::OutputJumpTable<FieldID, Field> jumpTable;
        for (auto& field : fields)
            jumpTable.Add(field.ID(), field);
        archive(jumpTable);
    }

    ::Inscription::Version OutputWorldArchiveInterface::CurrentVersion()
    {
        return 1;
    }
}