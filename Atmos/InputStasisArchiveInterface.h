#pragma once

#include "InputFieldArchiveInterface.h"

#include "Field.h"
#include "FilePath.h"

#include <Inscription/InputBinaryArchive.h>
#include <Inscription/InputJumpTable.h>

namespace Atmos::World::Serialization
{
    class InputStasisArchiveInterface final : public InputFieldArchiveInterface
    {
    public:
        explicit InputStasisArchiveInterface(const File::Path& filePath);

        [[nodiscard]] std::optional<Field> ExtractField(FieldID id, Arca::Reliquary& reliquary) override;
        [[nodiscard]] std::unique_ptr<Field> ExtractFieldAsHeap(FieldID id, Arca::Reliquary& reliquary) override;

        [[nodiscard]] std::vector<FieldID> AllFieldIDs() const override;
        [[nodiscard]] bool ContainsField(FieldID iD) const override;
        [[nodiscard]] size_t FieldSize() const override;
    protected:
        [[nodiscard]] ::Inscription::BinaryArchive& Archive() override;
        [[nodiscard]] const ::Inscription::BinaryArchive& Archive() const override;
    private:
        using ArchiveT = ::Inscription::InputBinaryArchive;
        ArchiveT archive;

        Inscription::InputJumpTable<FieldID, Field> fieldJumpTable;
    private:
        void Load();
    };
}