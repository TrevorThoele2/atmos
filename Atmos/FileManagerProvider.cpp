#include "FileManagerProvider.h"

#include "NullFileManager.h"

#include "FilePath.h"

namespace Atmos::File
{
    ManagerProvider::ManagerProvider()
    {
        value = std::make_unique<NullFileManager>();
    }

    void ManagerProvider::OnSetup()
    {
        CreateDefaultDirectories();
    }

    void ManagerProvider::CreateDefaultDirectories()
    {
        std::filesystem::create_directory(value->DataPath());
    }

    ManagerProvider manager;
}