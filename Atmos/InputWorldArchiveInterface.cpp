#include "InputWorldArchiveInterface.h"

#include "TypeRegistration.h"

namespace Atmos::World::Serialization
{
    InputWorldArchiveInterface::InputWorldArchiveInterface(const File::Path& filePath) :
        archive(filePath, "ATMOS WORLD")
    {
        Load();
    }

    std::optional<Field> InputWorldArchiveInterface::ExtractField(FieldID id, ExternalManagers externalManagers)
    {
        Arca::ReliquaryOrigin origin;
        RegisterFieldTypes(origin, *externalManagers.audio, *externalManagers.input, *externalManagers.graphics);

        Field out(id, origin.Actualize());
        fieldJumpTable.FillObject(id, out, archive);
        return std::move(out);
    }

    std::unique_ptr<Field> InputWorldArchiveInterface::ExtractFieldAsHeap(FieldID id, ExternalManagers externalManagers)
    {
        Arca::ReliquaryOrigin origin;
        RegisterFieldTypes(origin, *externalManagers.audio, *externalManagers.input, *externalManagers.graphics);

        auto out = std::make_unique<Field>(id, origin.Actualize());
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
        archive(fieldJumpTable);
    }
}