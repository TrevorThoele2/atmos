#include <SDKDDKVer.h>
#include <boost/process.hpp>
#include <boost/process/windows.hpp>

#include "VulkanShaderCompiler.h"

#include "GraphicsError.h"
#include "Logger.h"

namespace Atmos::Render::Vulkan
{
    ShaderCompiler::ShaderCompiler(Logging::Logger& logger) : logger(&logger)
    {}

    void ShaderCompiler::Compile(
        const File::Path& inputPath,
        const File::Path& outputPath)
    {
        DoCompile(inputPath, outputPath, { "-O" });
    }

    void ShaderCompiler::CompileWithDebugging(
        const File::Path& inputPath,
        const File::Path& outputPath)
    {
        DoCompile(inputPath, outputPath, { "-O0" });
    }

    void ShaderCompiler::DoCompile(
        const File::Path& inputPath,
        const File::Path& outputPath,
        const std::vector<std::string>& additionalFlags)
    {
        try
        {
            auto path = "./tools32/glslc.exe " + inputPath.string() + " -o " + outputPath.string();
            for (auto& flag : additionalFlags)
                path += " " + flag;

            boost::process::ipstream outStream;
            boost::process::ipstream errorStream;
            boost::process::child child(
                path,
                boost::process::std_out > outStream,
                boost::process::std_err > errorStream,
                boost::process::windows::hide);

            while (child.running())
            {
                std::string errorLine;
                if (std::getline(errorStream, errorLine) && !errorLine.empty())
                    throw GraphicsError("Shader compilation has encountered an error.",
                        { {"Details", errorLine } });
            }

            child.wait();
        }
        catch(...)
        {
            logger->Log(
                "Compilation of shader failed.",
                Logging::Severity::Information,
                { { { "InputPath", inputPath.generic_string() } } });
            throw;
        }

        logger->Log(
            "Compilation of shader succeeded.",
            Logging::Severity::Information,
            { { { "InputPath", inputPath.generic_string() } } });
    }
}
