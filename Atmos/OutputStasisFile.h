#pragma once

#include "Field.h"
#include "FilePath.h"
#include "Property.h"

#include "VersionUserContext.h"

#include <Inscription/OutputBinaryArchive.h>

namespace Atmos::World::Serialization
{
    class OutputStasisFile final
    {
    public:
        explicit OutputStasisFile(const File::Path& filePath);

        void Save(std::vector<Field> fields, const std::vector<Property>& properties);
    private:
        Inscription::File::OutputBinary file;
        Inscription::Archive::OutputBinary archive;
        Inscription::Format::OutputBinary format;

        VersionUserContext versionUserContext;

        static inline const std::string signature = "ATMOS STASIS";
    };
}
