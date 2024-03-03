#pragma once

#include "Field.h"

namespace Atmos::World::Serialization
{
    class InputFieldArchiveInterface
    {
    public:
        virtual ~InputFieldArchiveInterface() = 0;

        [[nodiscard]] virtual std::optional<Field> ExtractField(FieldID id, Arca::Reliquary& globalReliquary) = 0;
        [[nodiscard]] virtual std::unique_ptr<Field> ExtractFieldAsHeap(FieldID id, Arca::Reliquary& globalReliquary) = 0;

        [[nodiscard]] virtual std::vector<FieldID> AllFieldIDs() const = 0;
        [[nodiscard]] virtual bool ContainsField(FieldID fieldID) const = 0;
        [[nodiscard]] virtual size_t FieldSize() const = 0;
    protected:
        InputFieldArchiveInterface() = default;
    };
}
