#include "NullFileManager.h"

namespace Atmos::File
{
    void NullFileManager::MakeDirectory(const Path& path)
    {}

    bool NullFileManager::RelocateFile(const Path& from, const Path& to)
    {
        return false;
    }

    bool NullFileManager::RemoveFile(const Path& remove)
    {
        return false;
    }

    Path NullFileManager::ExePath() const
    {
        return Path();
    }

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

    String NullFileManager::FileSeparator() const
    {
        return "";
    }
}