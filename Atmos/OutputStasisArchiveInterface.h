#pragma once

#include "Field.h"
#include "FilePath.h"

#include <Inscription/OutputBinaryArchive.h>

namespace Atmos::World::Serialization
{
    class OutputStasisArchiveInterface final
    {
    public:
        explicit OutputStasisArchiveInterface(const File::Path& filePath);

        void Save(std::vector<Field> fields);

        static ::Inscription::Version CurrentVersion();
    private:
        using ArchiveT = ::Inscription::OutputBinaryArchive;
        ArchiveT archive;
    };
}
