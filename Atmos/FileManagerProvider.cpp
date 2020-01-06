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

    void ManagerProvider::OnSetup()
    {
        CreateDefaultDirectories();
        SetupFilePaths();
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

    ManagerProvider manager;
}