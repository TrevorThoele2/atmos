#pragma once

#include "ArchiveInterface.h"

#include <Arca/Reliquary.h>
#include "FilePath.h"

#include <Inscription/OutputBinaryArchive.h>

namespace Atmos::World::Serialization
{
    class OutputGlobalArchiveInterface final : public ArchiveInterface
    {
    public:
        static const char* const defaultExtension;
    public:
        explicit OutputGlobalArchiveInterface
        (
            const File::Path& filePath,
            Arca::Reliquary& globalReliquary
        );

        static ::Inscription::Version CurrentVersion();
    protected:
        [[nodiscard]] ::Inscription::BinaryArchive& Archive() override;
        [[nodiscard]] const ::Inscription::BinaryArchive& Archive() const override;
    private:
        using ArchiveT = ::Inscription::OutputBinaryArchive;
        ArchiveT archive;
    private:
        void Save(Arca::Reliquary& globalReliquary);
    };
}
