#include "InputStasisArchiveInterface.h"

#include "ArchiveHeader.h"

#include <Inscription/VectorScribe.h>

namespace Atmos::World::Serialization
{
    InputStasisArchiveInterface::InputStasisArchiveInterface(
        const File::Path& filePath,
        Inscription::LoadAssetsUserContext& loadAssetsUserContext)
        :
        archive(filePath)
    {
        archive.EmplaceUserContext(&loadAssetsUserContext);

        Load();
    }

    std::optional<Field> InputStasisArchiveInterface::ExtractField(
        FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary)
    {
        Field out{ id, std::move(reliquary) };
        fieldJumpTable.FillObject(id, out, archive);
        return std::move(out);
    }

    std::unique_ptr<Field> InputStasisArchiveInterface::ExtractFieldAsHeap(
        FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary)
    {
        auto out = std::make_unique<Field>(id, std::move(reliquary));
        fieldJumpTable.FillObject(id, *out, archive);
        return out;
    }

    std::vector<FieldID> InputStasisArchiveInterface::AllFieldIDs() const
    {
        return fieldJumpTable.AllIDs();
    }

    bool InputStasisArchiveInterface::ContainsField(FieldID iD) const
    {
        return fieldJumpTable.Contains(iD);
    }

    size_t InputStasisArchiveInterface::FieldSize() const
    {
        return fieldJumpTable.Size();
    }

    std::vector<Property> InputStasisArchiveInterface::WorldProperties() const
    {
        return worldProperties;
    }

    void InputStasisArchiveInterface::Load()
    {
        const auto header = LoadArchiveHeader(archive, expectedSignature);

        versionUserContext.inscriptionVersion = std::get<0>(header);
        versionUserContext.clientVersion = std::get<1>(header);

        archive.EmplaceUserContext(&versionUserContext);

        archive(worldProperties);

        archive(fieldJumpTable);
    }
}
