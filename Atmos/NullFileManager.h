#pragma once

#include "FileManager.h"

namespace Atmos::File
{
    class NullFileManager final : public FileManager
    {
    public:
        NullFileManager() = default;

        void MakeDirectory(const Path& path) override;
        bool RelocateFile(const Path& from, const Path& to) override;
        bool RemoveFile(const Path& remove) override;

        [[nodiscard]] Path ExePath() const override;
        [[nodiscard]] Path DataPath() const override;
        [[nodiscard]] Path TemporaryDirectoryPath() const override;
        [[nodiscard]] size_t MaxPathLength() const override;
        [[nodiscard]] String FileSeparator() const override;
    };
}