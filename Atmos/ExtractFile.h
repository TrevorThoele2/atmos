#pragma once

#include <Arca/Command.h>
#include "FilePath.h"
#include "String.h"
#include "Buffer.h"

namespace Atmos::File
{
    struct ExtractFile
    {
        Path filePath;
    };

    struct ExtractZippedFile
    {
        Path filePath;
        String name;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::File::ExtractFile>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::File::ExtractFile";
        using Result = Atmos::Buffer;
    };

    template<>
    struct Traits<Atmos::File::ExtractZippedFile>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::File::ExtractZippedFile";
        using Result = Atmos::Buffer;
    };
}