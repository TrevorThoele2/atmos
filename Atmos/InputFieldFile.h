#pragma once

#include "Field.h"
#include "Property.h"

#include <Arca/Reliquary.h>
#include "LoadAssetsUserContext.h"

namespace Atmos::World::Serialization
{
    class InputFieldFile
    {
    public:
        virtual ~InputFieldFile() = 0;

        [[nodiscard]] virtual std::optional<Field> ExtractField(
            FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary) = 0;
        [[nodiscard]] virtual std::unique_ptr<Field> ExtractFieldAsHeap(
            FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary) = 0;

        [[nodiscard]] virtual std::vector<FieldID> AllFieldIDs() const = 0;
        [[nodiscard]] virtual bool ContainsField(FieldID fieldID) const = 0;
        [[nodiscard]] virtual size_t FieldSize() const = 0;

        [[nodiscard]] virtual std::vector<Property> WorldProperties() const = 0;
    protected:
        InputFieldFile() = default;
    };
}
