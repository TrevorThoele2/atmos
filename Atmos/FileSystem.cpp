#include "FileSystem.h"

#include "NullFileManager.h"

#include "FilePath.h"
#include "RelativeFilePath.h"

namespace Atmos::File
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
        Path::fileSeparator = fileSeparator;
        RelativePath::fileSeparator = fileSeparator;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::File::FileSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::UniqueProviderSystem<::Atmos::File::FileManager>>(object, archive);
    }
}