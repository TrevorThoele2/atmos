#pragma once

#include "FileManager.h"

#include "WindowsInclude.h"

namespace Atmos
{
    class WindowsFileManager : public FileManager
    {
    public:
        void MakeDirectory(const FilePath& path) override;
        bool RelocateFile(const FilePath& from, const FilePath& to) override;
        bool RemoveFile(const FilePath& remove) override;

        FilePath ExePath() const override;
        FilePath DataPath() const override;
        FilePath TemporaryDirectoryPath() const override;
        size_t MaxPathLength() const override;
        String FileSeparator() const override;
    private:
        static constexpr size_t maxPathLength = MAX_PATH * sizeof(TCHAR) + sizeof(TCHAR);
    };
}