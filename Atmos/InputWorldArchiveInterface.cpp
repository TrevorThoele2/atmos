#include "InputWorldArchiveInterface.h"

namespace Atmos::World::Serialization
{
    InputWorldArchiveInterface::InputWorldArchiveInterface(const File::Path& filePath) :
        InputFieldArchiveInterface(filePath),
        archive(filePath, "ATMOS WORLD")
    {
        Load();
    }

    std::optional<Field> InputWorldArchiveInterface::ExtractField(FieldID id, Arca::Reliquary& reliquary)
    {
        Field out(id, reliquary);
        fieldJumpTable.FillObject(id, out, archive);
        return std::move(out);
    }

    std::unique_ptr<Field> InputWorldArchiveInterface::ExtractFieldAsHeap(FieldID id, Arca::Reliquary& reliquary)
    {
        auto out = std::make_unique<Field>(id, reliquary);
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

    ::Inscription::BinaryArchive& InputWorldArchiveInterface::Archive()
    {
        return archive;
    }

    const ::Inscription::BinaryArchive& InputWorldArchiveInterface::Archive() const
    {
        return archive;
    }

    void InputWorldArchiveInterface::Load()
    {
        archive(fieldJumpTable);
    }
}