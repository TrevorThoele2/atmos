#pragma once

#include "FilePath.h"
#include "Logger.h"
#include "Buffer.h"

namespace Atmos::Render::Vulkan
{
    class ShaderCompiler
    {
    public:
        explicit ShaderCompiler(Logging::Logger& logger);

        Buffer Compile(const File::Path& filePath);
        Buffer CompileWithDebugging(const File::Path& filePath);
    private:
        Buffer DoCompile(
            const File::Path& inputPath,
            const std::vector<std::string>& additionalFlags = {});
    private:
        Logging::Logger* logger;
    };
}
