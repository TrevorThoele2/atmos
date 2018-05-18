
#include <fstream>
#include "FileUtility.h"

namespace Atmos
{
    bool DoesFileExist(const FilePath &path)
    {
        std::ifstream checker(path.c_str(), std::ios_base::in | std::ios_base::app);
        return checker.good();
    }
}