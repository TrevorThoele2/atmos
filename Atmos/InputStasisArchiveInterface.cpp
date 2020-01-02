#include "InputStasisArchiveInterface.h"

#include "TypeRegistration.h"

namespace Atmos::World::Serialization
{
    InputStasisArchiveInterface::InputStasisArchiveInterface(const File::Path& filePath) :
        archive(filePath, "ATMOS STASIS")
    {
        Load();
    }

    std::optional<Field> InputStasisArchiveInterface::ExtractField(FieldID id, Arca::Reliquary& globalReliquary)
    {
        Arca::ReliquaryOrigin origin;
        RegisterFieldTypes(origin, globalReliquary);

        Field out{ id, origin.Actualize() };
        fieldJumpTable.FillObject(id, out, archive);
        return std::move(out);
    }

    std::unique_ptr<Field> InputStasisArchiveInterface::ExtractFieldAsHeap(FieldID id, Arca::Reliquary& globalReliquary)
    {
        Arca::ReliquaryOrigin origin;
        RegisterFieldTypes(origin, globalReliquary);

        auto out = std::make_unique<Field>(id, origin.Actualize());
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

    void InputStasisArchiveInterface::Load()
    {
        archive(fieldJumpTable);
    }
}
