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

        virtual void MakeDirectory(const Path& path) = 0;
        virtual bool RelocateFile(const Path& from, const Path& to) = 0;
        virtual bool RemoveFile(const Path& remove) = 0;

        [[nodiscard]] virtual Path ExePath() const = 0;
        [[nodiscard]] virtual Path DataPath() const = 0;
        [[nodiscard]] virtual Path TemporaryDirectoryPath() const = 0;
        [[nodiscard]] virtual size_t MaxPathLength() const = 0;
        [[nodiscard]] virtual String FileSeparator() const = 0;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::File::FileManager, BinaryArchive> final :
        public CompositeScribe<::Atmos::File::FileManager, BinaryArchive>
    {};
}