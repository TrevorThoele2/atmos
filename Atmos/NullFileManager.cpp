
#include "NullFileManager.h"

namespace Atmos
{
    NullFileManager::NullFileManager()
    {}

    void NullFileManager::MakeDirectory(const FilePath& path)
    {}

    bool NullFileManager::RelocateFile(const FilePath& from, const FilePath& to)
    {
        return false;
    }

    bool NullFileManager::RemoveFile(const FilePath& remove)
    {
        return false;
    }

    FilePath NullFileManager::ExePath() const
    {
        return FilePath();
    }

    FilePath NullFileManager::DataPath() const
    {
        return FilePath();
    }

    FilePath NullFileManager::TemporaryDirectoryPath() const
    {
        return FilePath();
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