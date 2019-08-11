#pragma once

#include "FileManager.h"

#include "WindowsInclude.h"

namespace Atmos::File
{
    class WindowsFileManager : public FileManager
    {
    public:
        void MakeDirectory(const Path& path) override;
        bool RelocateFile(const Path& from, const Path& to) override;
        bool RemoveFile(const Path& remove) override;

        Path ExePath() const override;
        Path DataPath() const override;
        Path TemporaryDirectoryPath() const override;
        size_t MaxPathLength() const override;
        String FileSeparator() const override;
    private:
        static constexpr size_t maxPathLength = MAX_PATH * sizeof(TCHAR) + sizeof(TCHAR);
    };
}