#pragma once

#include <memory>

#include "FilePath.h"
#include "String.h"

namespace Atmos
{
    class FileManager
    {
    public:
        virtual ~FileManager() = 0;

        virtual void MakeDirectory(const FilePath& path) = 0;
        virtual bool RelocateFile(const FilePath& from, const FilePath& to) = 0;
        virtual bool RemoveFile(const FilePath& remove) = 0;

        virtual FilePath ExePath() const = 0;
        virtual FilePath DataPath() const = 0;
        virtual FilePath TemporaryDirectoryPath() const = 0;
        virtual size_t MaxPathLength() const = 0;
        virtual String FileSeparator() const = 0;
    };
}