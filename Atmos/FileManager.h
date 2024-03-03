#pragma once

#include "FilePath.h"

#include "Serialization.h"

namespace Atmos::File
{
    class FileManager
    {
    public:
        virtual ~FileManager() = 0;

        [[nodiscard]] virtual Path DataPath() const = 0;
        [[nodiscard]] virtual Path TemporaryDirectoryPath() const = 0;
        [[nodiscard]] virtual size_t MaxPathLength() const = 0;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::File::FileManager, Archive> final
    {
        using Category = NullScribeCategory<Atmos::File::FileManager>;
    };
}