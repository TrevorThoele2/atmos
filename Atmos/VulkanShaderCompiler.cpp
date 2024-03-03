#include "VulkanShaderCompiler.h"

#include <SDKDDKVer.h>
#include <boost/process.hpp>
#include <boost/process/windows.hpp>

#include "GraphicsError.h"
#include "Log.h"

namespace Atmos::Render::Vulkan
{
    void ShaderCompiler::Compile(
        const File::Path& inputPath,
        const File::Path& outputPath)
    {
        boost::process::ipstream outStream;
        boost::process::ipstream errorStream;
        boost::process::child child(
            "./Tools32/glslc.exe " + inputPath.string() + " -o " + outputPath.string() + " -O",
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
}
