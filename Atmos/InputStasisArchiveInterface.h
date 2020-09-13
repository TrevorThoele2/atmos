#pragma once

#include "InputFieldArchiveInterface.h"

#include "Field.h"
#include "FilePath.h"

#include "VersionUserContext.h"

#include <Inscription/InputBinaryArchive.h>
#include <Inscription/InputJumpTable.h>

namespace Atmos::World::Serialization
{
    class InputStasisArchiveInterface final : public InputFieldArchiveInterface
    {
    public:
        explicit InputStasisArchiveInterface(
            const File::Path& filePath,
            Inscription::LoadAssetsUserContext& loadAssetsUserContext);

        [[nodiscard]] std::optional<Field> ExtractField(
            FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary) override;
        [[nodiscard]] std::unique_ptr<Field> ExtractFieldAsHeap(
            FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary) override;

        [[nodiscard]] std::vector<FieldID> AllFieldIDs() const override;
        [[nodiscard]] bool ContainsField(FieldID iD) const override;
        [[nodiscard]] size_t FieldSize() const override;
    private:
        using ArchiveT = Inscription::InputBinaryArchive;
        ArchiveT archive;

        Inscription::InputJumpTable<FieldID, Field> fieldJumpTable;

        VersionUserContext versionUserContext;

        static inline const std::string expectedSignature = "ATMOS STASIS";
    private:
        void Load();
    };
}