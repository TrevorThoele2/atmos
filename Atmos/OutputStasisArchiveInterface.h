#pragma once

#include "Field.h"
#include "FilePath.h"

#include "VersionUserContext.h"

#include <Inscription/OutputBinaryArchive.h>

namespace Atmos::World::Serialization
{
    class OutputStasisArchiveInterface final
    {
    public:
        explicit OutputStasisArchiveInterface(const File::Path& filePath);

        void Save(std::vector<Field> fields);
    private:
        using ArchiveT = ::Inscription::OutputBinaryArchive;
        ArchiveT archive;

        VersionUserContext versionUserContext;

        static inline const std::string signature = "ATMOS STASIS";
    };
}
