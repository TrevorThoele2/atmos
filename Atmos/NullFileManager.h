#pragma once

#include "FileManager.h"

namespace Atmos
{
    class NullFileManager : public FileManager
    {
    public:
        NullFileManager();

        void MakeDirectory(const FilePath& path) override;
        bool RelocateFile(const FilePath& from, const FilePath& to) override;
        bool RemoveFile(const FilePath& remove) override;

        FilePath ExePath() const override;
        FilePath DataPath() const override;
        FilePath TemporaryDirectoryPath() const override;
        size_t MaxPathLength() const override;
        String FileSeparator() const override;
    };
}