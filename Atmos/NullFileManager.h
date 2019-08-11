#pragma once

#include "FileManager.h"

namespace Atmos::File
{
    class NullFileManager : public FileManager
    {
    public:
        NullFileManager();

        void MakeDirectory(const Path& path) override;
        bool RelocateFile(const Path& from, const Path& to) override;
        bool RemoveFile(const Path& remove) override;

        Path ExePath() const override;
        Path DataPath() const override;
        Path TemporaryDirectoryPath() const override;
        size_t MaxPathLength() const override;
        String FileSeparator() const override;
    };
}