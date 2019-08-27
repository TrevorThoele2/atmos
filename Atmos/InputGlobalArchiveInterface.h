#pragma once

#include "ArchiveInterface.h"

#include <Arca/Reliquary.h>
#include "FilePath.h"

#include <Inscription/InputBinaryArchive.h>

namespace Atmos::World::Serialization
{
    class InputGlobalArchiveInterface final : public ArchiveInterface
    {
    public:
        explicit InputGlobalArchiveInterface(const File::Path& filePath);

        std::unique_ptr<Arca::Reliquary> TakeGlobalReliquary();
    protected:
        [[nodiscard]] ::Inscription::BinaryArchive& Archive() override;
        [[nodiscard]] const ::Inscription::BinaryArchive& Archive() const override;
    private:
        using ArchiveT = ::Inscription::InputBinaryArchive;
        ArchiveT archive;

        std::unique_ptr<Arca::Reliquary> globalReliquary;
    private:
        void Load();
    };
}