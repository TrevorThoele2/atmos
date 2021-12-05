#include "VulkanShaderCompiler.h"

#include "Process.h"
#include "GraphicsError.h"
#include "Logger.h"

#include <Inscription/InputBinaryArchive.h>

namespace Atmos::Render::Vulkan
{
    ShaderCompiler::ShaderCompiler(Logging::Logger& logger) : logger(&logger)
    {}

    Buffer ShaderCompiler::Compile(const File::Path& filePath)
    {
        return DoCompile(filePath, { "-O" });
    }

    Buffer ShaderCompiler::CompileWithDebugging(const File::Path& filePath)
    {
        return DoCompile(filePath, { "-O0" });
    }
    
    Buffer ShaderCompiler::DoCompile(
        const File::Path& inputPath,
        const std::vector<std::string>& additionalFlags)
    {
        const auto outputPath = File::Path(inputPath).replace_extension(
            inputPath.extension().string() + String("_temp"));
        try
        {
            auto path = "./tools32/glslc.exe " + inputPath.string() + " -o " + outputPath.string();
            for (auto& flag : additionalFlags)
                path += " " + flag;

            const auto output = Process::Run(path);

            if (!output.error.empty())
                throw GraphicsError(
                    "Shader compilation has encountered an error.",
                    { { "Details", output.error } });
        }
        catch(...)
        {
            logger->Log(
                "Compilation of shader failed.",
                Logging::Severity::Information,
                { { { "InputPath", inputPath.generic_string() } } });
            throw;
        }

        Inscription::Buffer buffer;
        {
            Inscription::File::InputBinary file(outputPath);
            Inscription::Archive::InputBinary archive(file);
            Inscription::Format::InputBinary format(archive);
            buffer.value.resize(static_cast<const unsigned int>(file.Size()));
            format.Read(buffer);
        }

        std::filesystem::remove(outputPath);

        logger->Log(
            "Compilation of shader succeeded.",
            Logging::Severity::Information,
            { { { "InputPath", inputPath.generic_string() } } });

        return buffer.value;
    }
}
