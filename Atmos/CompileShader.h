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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::CompileShader";
        using Result = Atmos::Buffer;
    };
}