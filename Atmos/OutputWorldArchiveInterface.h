#pragma once

#include "ArchiveInterface.h"

#include "Field.h"
#include "FilePath.h"

#include <Inscription/OutputBinaryArchive.h>

namespace Atmos::World::Serialization
{
    class OutputWorldArchiveInterface final : public ArchiveInterface
    {
    public:
        OutputWorldArchiveInterface
        (
            const File::Path& filePath,
            std::vector<Field>& fields
        );

        static ::Inscription::Version CurrentVersion();
    protected:
        [[nodiscard]] ::Inscription::BinaryArchive& Archive() override;
        [[nodiscard]] const ::Inscription::BinaryArchive& Archive() const override;
    private:
        using ArchiveT = ::Inscription::OutputBinaryArchive;
        ArchiveT archive;
    private:
        void Save(std::vector<Field>& fields);
    };
}