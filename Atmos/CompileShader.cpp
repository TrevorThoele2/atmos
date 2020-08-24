#include "CompileShader.h"

namespace Atmos::Render
{
    CompileShader::CompileShader(const File::Path& inputFilePath) :
        inputFilePath(inputFilePath)
    {}

    CompileShader::CompileShader(const File::Path& inputFilePath, const File::Path& outputFilePath) :
        inputFilePath(inputFilePath), outputFilePath(outputFilePath)
    {}
}