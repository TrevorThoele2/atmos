#include <sdkddkver.h>
#include <boost/process.hpp>
#include <boost/process/windows.hpp>

#include "VulkanShaderCompiler.h"

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

        Inscription::Buffer buffer;
        {
            Inscription::Archive::InputBinary archive(outputPath);
            buffer.value.resize(static_cast<const unsigned int>(archive.Size()));
            archive.Read(buffer);
        }

        logger->Log(
            "Compilation of shader succeeded.",
            Logging::Severity::Information,
            { { { "InputPath", inputPath.generic_string() } } });

        return buffer.value;
    }
}
