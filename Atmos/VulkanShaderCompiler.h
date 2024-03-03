#pragma once

#include "FilePath.h"

namespace Atmos::Render::Vulkan
{
    class ShaderCompiler
    {
    public:
        void Compile(
            const File::Path& inputPath,
            const File::Path& outputPath);
    };
}
