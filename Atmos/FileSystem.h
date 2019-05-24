#pragma once

#include "UniqueProviderSystem.h"
#include "FileManager.h"

namespace Atmos
{
    class FileSystem : public UniqueProviderSystem<FileManager>
    {
    public:
        FileSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(FileSystem);
    private:
        void InitializeImpl() override;
    private:
        void CreateDefaultDirectories();
        void SetupFilePaths();
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::FileSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}