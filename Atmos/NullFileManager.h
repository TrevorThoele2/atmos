#pragma once

#include "FileManager.h"

namespace Atmos::File
{
    class NullFileManager final : public FileManager
    {
    public:
        NullFileManager() = default;

        [[nodiscard]] Path DataPath() const override;
        [[nodiscard]] Path TemporaryDirectoryPath() const override;
        [[nodiscard]] size_t MaxPathLength() const override;
    };
}