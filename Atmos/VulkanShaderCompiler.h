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
        void CompileWithDebugging(
            const File::Path& inputPath,
            const File::Path& outputPath);
    private:
        void DoCompile(
            const File::Path& inputPath,
            const File::Path& outputPath,
            const std::vector<std::string>& additionalFlags = {});
    };
}
