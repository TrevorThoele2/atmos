#include "OutputStasisFile.h"

#include <Inscription/VectorScribe.h>
#include <Inscription/OutputJumpTable.h>

#include "FileHeader.h"

namespace Atmos::World::Serialization
{
    OutputStasisFile::OutputStasisFile(const File::Path& filePath) :
        file(filePath), archive(file), format(archive), versionUserContext{ Inscription::currentInscriptionVersion, 1 }
    {}

    void OutputStasisFile::Save(std::vector<Field> fields, const std::vector<Property>& properties)
    {
        FileHeader header{ signature, signature, versionUserContext.inscriptionVersion, versionUserContext.clientVersion };
        format("header", header);

        format.EmplaceUserContext(&versionUserContext);

        format(properties);

        Inscription::OutputJumpTable<FieldID, Field> jumpTable;
        for (auto& field : fields)
            jumpTable.Add(field.ID(), field);
        jumpTable.Save(file, format);
    }
}