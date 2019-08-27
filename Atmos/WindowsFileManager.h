#pragma once

#include "FileManager.h"

#include "WindowsInclude.h"

namespace Atmos::File
{
    class WindowsFileManager final : public FileManager
    {
    public:
        void MakeDirectory(const Path& path) override;
        bool RelocateFile(const Path& from, const Path& to) override;
        bool RemoveFile(const Path& remove) override;

        [[nodiscard]] Path ExePath() const override;
        [[nodiscard]] Path DataPath() const override;
        [[nodiscard]] Path TemporaryDirectoryPath() const override;
        [[nodiscard]] size_t MaxPathLength() const override;
        [[nodiscard]] String FileSeparator() const override;
    private:
        constexpr static size_t maxPathLength = MAX_PATH * sizeof(TCHAR) + sizeof(TCHAR);
    };
}