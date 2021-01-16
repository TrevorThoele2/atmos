#include "OutputStasisArchiveInterface.h"

#include <Inscription/VectorScribe.h>
#include <Inscription/OutputJumpTable.h>

#include "ArchiveHeader.h"

namespace Atmos::World::Serialization
{
    OutputStasisArchiveInterface::OutputStasisArchiveInterface(const File::Path& filePath) :
        archive(filePath), versionUserContext{ Inscription::currentInscriptionVersion, 1 }
    {}

    void OutputStasisArchiveInterface::Save(std::vector<Field> fields, const std::vector<Property>& properties)
    {
        SaveArchiveHeader(
            archive,
            signature,
            versionUserContext.inscriptionVersion,
            versionUserContext.clientVersion);

        archive.EmplaceUserContext(&versionUserContext);

        archive(properties);

        Inscription::OutputJumpTable<FieldID, Field> jumpTable;
        for (auto& field : fields)
            jumpTable.Add(field.ID(), field);
        archive(jumpTable);
    }
}