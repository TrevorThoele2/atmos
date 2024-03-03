#include <fstream>
#include "FileUtility.h"

namespace Atmos::File
{
    bool DoesFileExist(const Path& path)
    {
        const std::ifstream checker(path.c_str(), std::ios_base::in | std::ios_base::app);
        return checker.good();
    }
}