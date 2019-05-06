
#include "FileSystem.h"

#include "NullFileManager.h"

#include "FilePath.h"
#include "RelativeFilePath.h"

namespace Atmos
{
    FileSystem::FileSystem(ObjectManager& manager) : UniqueProviderSystem(manager, UniqueProviderSystem::ValuePtr(new NullFileManager()))
    {}

    FileSystem::FileSystem(const ::Inscription::Table<FileSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(UniqueProviderSystem<FileManager>)
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
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::FileSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::UniqueProviderSystem<::Atmos::FileManager>);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::FileSystem, "FileSystem");
}