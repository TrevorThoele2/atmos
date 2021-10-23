#pragma once

#include <Arca/Command.h>

#include "FilePath.h"
#include "Buffer.h"

namespace Atmos::Render
{
    struct CompileShader
    {
        File::Path filePath;

        CompileShader(const File::Path& filePath);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::CompileShader>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Render::CompileShader"; }
        using Result = Atmos::Buffer;
    };
}