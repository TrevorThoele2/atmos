#include "InputWorldFile.h"

#include "FileHeader.h"

#include <Inscription/VectorScribe.h>

namespace Atmos::World::Serialization
{
    InputWorldFile::InputWorldFile(
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

    std::optional<Field> InputWorldFile::ExtractField(
        FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary)
    {
        Field out(id, std::move(reliquary));
        fieldJumpTable.FillObject(id, out, format);
        return std::move(out);
    }

    std::unique_ptr<Field> InputWorldFile::ExtractFieldAsHeap(
        FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary)
    {
        auto out = std::make_unique<Field>(id, std::move(reliquary));
        fieldJumpTable.FillObject(id, *out, format);
        return out;
    }

    std::vector<FieldID> InputWorldFile::AllFieldIDs() const
    {
        return fieldJumpTable.AllIDs();
    }

    bool InputWorldFile::ContainsField(FieldID fieldID) const
    {
        return fieldJumpTable.Contains(fieldID);
    }

    size_t InputWorldFile::FieldSize() const
    {
        return fieldJumpTable.Size();
    }

    std::vector<Property> InputWorldFile::WorldProperties() const
    {
        return worldProperties;
    }

    void InputWorldFile::Load()
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