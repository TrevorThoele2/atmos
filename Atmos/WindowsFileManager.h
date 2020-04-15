#pragma once

#include "FileManager.h"

#include "WindowsInclude.h"

namespace Atmos::File
{
    class WindowsFileManager final : public FileManager
    {
    public:
        [[nodiscard]] Path DataPath() const override;
        [[nodiscard]] Path TemporaryDirectoryPath() const override;
        [[nodiscard]] size_t MaxPathLength() const override;
    private:
        constexpr static size_t maxPathLength = MAX_PATH * sizeof(TCHAR) + sizeof(TCHAR);
    };
}