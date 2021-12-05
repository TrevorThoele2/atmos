#pragma once

#include "InputFieldFile.h"

#include "Field.h"
#include "FilePath.h"

#include "VersionUserContext.h"

#include <Inscription/InputBinaryArchive.h>
#include <Inscription/InputJumpTable.h>

namespace Atmos::World::Serialization
{
    class InputWorldFile final : public InputFieldFile
    {
    public:
        explicit InputWorldFile(
            const File::Path& filePath,
            Inscription::LoadAssetsUserContext& loadAssetsUserContext);

        [[nodiscard]] std::optional<Field> ExtractField(
            FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary) override;
        [[nodiscard]] std::unique_ptr<Field> ExtractFieldAsHeap(
            FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary) override;

        [[nodiscard]] std::vector<FieldID> AllFieldIDs() const override;
        [[nodiscard]] bool ContainsField(FieldID fieldID) const override;
        [[nodiscard]] size_t FieldSize() const override;

        [[nodiscard]] std::vector<Property> WorldProperties() const override;
    private:
        Inscription::File::InputBinary file;
        Inscription::Archive::InputBinary archive;
        Inscription::Format::InputBinary format;

        Inscription::InputJumpTable<FieldID, Field> fieldJumpTable;

        std::vector<Property> worldProperties;

        VersionUserContext versionUserContext;

        static inline const std::string expectedSignature = "ATMOS WORLD";
    private:
        void Load();
    };
}