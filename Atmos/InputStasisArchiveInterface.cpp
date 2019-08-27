#include "InputStasisArchiveInterface.h"

namespace Atmos::World::Serialization
{
    InputStasisArchiveInterface::InputStasisArchiveInterface(const File::Path& filePath) :
        InputFieldArchiveInterface(filePath),
        archive(filePath, "ATMOS STASIS")
    {
        Load();
    }

    std::optional<Field> InputStasisArchiveInterface::ExtractField(FieldID id, Arca::Reliquary& reliquary)
    {
        Field out{ id, reliquary };
        fieldJumpTable.FillObject(id, out, archive);
        return std::move(out);
    }

    std::unique_ptr<Field> InputStasisArchiveInterface::ExtractFieldAsHeap(FieldID id, Arca::Reliquary& reliquary)
    {
        auto out = std::make_unique<Field>(id, reliquary);
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

    ::Inscription::BinaryArchive& InputStasisArchiveInterface::Archive()
    {
        return archive;
    }

    const ::Inscription::BinaryArchive& InputStasisArchiveInterface::Archive() const
    {
        return archive;
    }

    void InputStasisArchiveInterface::Load()
    {
        archive(fieldJumpTable);
    }
}