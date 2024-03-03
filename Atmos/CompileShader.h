#pragma once

#include <Arca/Command.h>

#include "FilePath.h"

namespace Atmos::Render
{
    struct CompileShader
    {
        File::Path inputFilePath;
        std::optional<File::Path> outputFilePath;

        CompileShader(const File::Path& inputFilePath);
        CompileShader(const File::Path& inputFilePath, const File::Path& outputFilePath);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::CompileShader>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Render::CompileShader"; }
        using Result = Atmos::File::Path;
    };
}