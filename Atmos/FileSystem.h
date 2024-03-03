#pragma once

#include "UniqueProviderSystem.h"
#include "FileManager.h"

namespace Atmos
{
    class FileSystem : public UniqueProviderSystem<FileManager>
    {
    public:
        FileSystem(ObjectManager& manager);
        FileSystem(const ::Inscription::Table<FileSystem>& table);
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
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}