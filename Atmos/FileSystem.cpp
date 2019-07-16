#include "FileSystem.h"

#include "NullFileManager.h"

#include "FilePath.h"
#include "RelativeFilePath.h"

namespace Atmos
{
    FileSystem::FileSystem(ObjectManager& manager) :
        UniqueProviderSystem(manager, UniqueProviderSystem::ValuePtr(new NullFileManager()))
    {}

    void FileSystem::InitializeImpl()
    {
        SetupFilePaths();
        CreateDefaultDirectories();
    }

    void FileSystem::CreateDefaultDirectories()
    {
        Get()->MakeDirectory(Get()->DataPath());
    }

    void FileSystem::SetupFilePaths()
    {
        auto fileSeparator = Get()->FileSeparator();
        FilePath::fileSeparator = fileSeparator;
        RelativeFilePath::fileSeparator = fileSeparator;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::FileSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::UniqueProviderSystem<::Atmos::FileManager>>(object, archive);
    }
}