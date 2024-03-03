#include "FileManagerProvider.h"

#include "NullFileManager.h"

#include "FilePath.h"
#include "RelativeFilePath.h"

namespace Atmos::File
{
    ManagerProvider::ManagerProvider()
    {
        value = std::make_unique<NullFileManager>();
    }

    FileManager& ManagerProvider::operator*()
    {
        return *value.get();
    }

    const FileManager& ManagerProvider::operator*() const
    {
        return *value.get();
    }

    FileManager* ManagerProvider::operator->()
    {
        return value.get();
    }

    const FileManager* ManagerProvider::operator->() const
    {
        return value.get();
    }

    void ManagerProvider::CreateDefaultDirectories()
    {
        value->MakeDirectory(value->DataPath());
    }

    void ManagerProvider::SetupFilePaths()
    {
        const auto fileSeparator = value->FileSeparator();
        Path::fileSeparator = fileSeparator;
        RelativePath::fileSeparator = fileSeparator;
    }
}