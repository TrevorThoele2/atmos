#include "OutputStasisArchiveInterface.h"

#include <Inscription/OutputJumpTable.h>

namespace Atmos::World::Serialization
{
    OutputStasisArchiveInterface::OutputStasisArchiveInterface(const File::Path& filePath) :
        archive(filePath, "ATMOS STASIS", CurrentVersion())
    {}

    void OutputStasisArchiveInterface::Save(std::vector<Field> fields)
    {
        ::Inscription::OutputJumpTable<FieldID, Field> jumpTable;
        for (auto& field : fields)
            jumpTable.Add(field.ID(), field);
        archive(jumpTable);
    }

    ::Inscription::Version OutputStasisArchiveInterface::CurrentVersion()
    {
        return 1;
    }
}