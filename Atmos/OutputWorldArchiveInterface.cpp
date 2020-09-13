#include "OutputWorldArchiveInterface.h"

#include <Inscription/OutputJumpTable.h>

#include "ArchiveHeader.h"

namespace Atmos::World::Serialization
{
    OutputWorldArchiveInterface::OutputWorldArchiveInterface(const File::Path& filePath) :
        archive(filePath), versionUserContext{ Inscription::currentInscriptionVersion, 1 }
    {}

    void OutputWorldArchiveInterface::Save(std::vector<Field>& fields)
    {
        SaveArchiveHeader(
            archive,
            signature,
            versionUserContext.inscriptionVersion,
            versionUserContext.clientVersion);

        archive.EmplaceUserContext(&versionUserContext);

        Inscription::OutputJumpTable<FieldID, Field> jumpTable;
        for (auto& field : fields)
            jumpTable.Add(field.ID(), field);
        archive(jumpTable);
    }
}