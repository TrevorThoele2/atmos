#include "SimpleFile.h"

#include <fstream>

namespace Atmos
{
    size_t FileSize(const File::Path& path)
    {
        std::ifstream stream(path, std::ios::in | std::ios::app | std::ios::ate);
        return static_cast<size_t>(stream.tellg());
    }
}