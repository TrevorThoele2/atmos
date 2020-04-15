#pragma once

#include "FilePath.h"
#include "String.h"

#include "Serialization.h"

namespace Atmos::File
{
    class FileManager
    {
    public:
        virtual ~FileManager() = 0;

        [[nodiscard]] virtual Path DataPath() const = 0;
        [[nodiscard]] virtual Path TemporaryDirectoryPath() const = 0;
        [[nodiscard]] virtual size_t MaxPathLength() const = 0;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::File::FileManager, BinaryArchive> final :
        public CompositeScribe<::Atmos::File::FileManager, BinaryArchive>
    {};
}