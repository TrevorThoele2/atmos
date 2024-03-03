#include "InputWorldArchiveInterface.h"

#include "ArchiveHeader.h"

namespace Atmos::World::Serialization
{
    InputWorldArchiveInterface::InputWorldArchiveInterface(
        const File::Path& filePath,
        Inscription::LoadAssetsUserContext& loadAssetsUserContext)
        :
        archive(filePath)
    {
        archive.EmplaceUserContext(&loadAssetsUserContext);

        Load();
    }

    std::optional<Field> InputWorldArchiveInterface::ExtractField(
        FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary)
    {
        Field out(id, std::move(reliquary));
        fieldJumpTable.FillObject(id, out, archive);
        return std::move(out);
    }

    std::unique_ptr<Field> InputWorldArchiveInterface::ExtractFieldAsHeap(
        FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary)
    {
        auto out = std::make_unique<Field>(id, std::move(reliquary));
        fieldJumpTable.FillObject(id, *out, archive);
        return out;
    }

    std::vector<FieldID> InputWorldArchiveInterface::AllFieldIDs() const
    {
        return fieldJumpTable.AllIDs();
    }

    bool InputWorldArchiveInterface::ContainsField(FieldID fieldID) const
    {
        return fieldJumpTable.Contains(fieldID);
    }

    size_t InputWorldArchiveInterface::FieldSize() const
    {
        return fieldJumpTable.Size();
    }

    void InputWorldArchiveInterface::Load()
    {
        const auto header = LoadArchiveHeader(archive, expectedSignature);

        versionUserContext.inscriptionVersion = std::get<0>(header);
        versionUserContext.clientVersion = std::get<1>(header);

        archive.EmplaceUserContext(&versionUserContext);

        archive(fieldJumpTable);
    }
}