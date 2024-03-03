#pragma once

#include "Field.h"
#include "FilePath.h"
#include "Property.h"

#include "VersionUserContext.h"

#include <Inscription/OutputBinaryArchive.h>

namespace Atmos::World::Serialization
{
    class OutputWorldArchiveInterface final
    {
    public:
        explicit OutputWorldArchiveInterface(const File::Path& filePath);

        void Save(std::vector<Field>& fields, const std::vector<Property>& worldProperties);
    private:
        using ArchiveT = Inscription::OutputBinaryArchive;
        ArchiveT archive;

        VersionUserContext versionUserContext;

        static inline const std::string signature = "ATMOS WORLD";
    };
}