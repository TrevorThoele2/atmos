#include "InputStasisFile.h"

#include "FileHeader.h"

#include <Inscription/VectorScribe.h>

namespace Atmos::World::Serialization
{
    InputStasisFile::InputStasisFile(
        const File::Path& filePath,
        Inscription::LoadAssetsUserContext& loadAssetsUserContext)
        :
        file(filePath),
        archive(file),
        format(archive)
    {
        format.EmplaceUserContext(&loadAssetsUserContext);

        Load();
    }

    std::optional<Field> InputStasisFile::ExtractField(
        FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary)
    {
        Field out{ id, std::move(reliquary) };
        fieldJumpTable.FillObject(id, out, format);
        return std::move(out);
    }

    std::unique_ptr<Field> InputStasisFile::ExtractFieldAsHeap(
        FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary)
    {
        auto out = std::make_unique<Field>(id, std::move(reliquary));
        fieldJumpTable.FillObject(id, *out, format);
        return out;
    }

    std::vector<FieldID> InputStasisFile::AllFieldIDs() const
    {
        return fieldJumpTable.AllIDs();
    }

    bool InputStasisFile::ContainsField(FieldID iD) const
    {
        return fieldJumpTable.Contains(iD);
    }

    size_t InputStasisFile::FieldSize() const
    {
        return fieldJumpTable.Size();
    }

    std::vector<Property> InputStasisFile::WorldProperties() const
    {
        return worldProperties;
    }

    void InputStasisFile::Load()
    {
        FileHeader header;
        header.expectedSignature = expectedSignature;

        format("header", header);

        versionUserContext.inscriptionVersion = header.inscriptionVersion;
        versionUserContext.clientVersion = header.clientVersion;

        format.EmplaceUserContext(&versionUserContext);

        format(worldProperties);

        fieldJumpTable.Load(file, format);
    }
}
