#include "NullFileManager.h"

namespace Atmos::File
{
    Path NullFileManager::DataPath() const
    {
        return Path();
    }

    Path NullFileManager::TemporaryDirectoryPath() const
    {
        return Path();
    }

    size_t NullFileManager::MaxPathLength() const
    {
        return 0;
    }
}