#pragma once

#include <Arca/Command.h>

#include "FilePath.h"

namespace Atmos::Scripting
{
    struct Compile
    {
        File::Path inputFilePath;
        std::optional<File::Path> outputFilePath;

        Compile(const File::Path& inputFilePath);
        Compile(const File::Path& inputFilePath, const File::Path& outputFilePath);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Scripting::Compile>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Scripting::Compile";
        using Result = Atmos::File::Path;
    };
}