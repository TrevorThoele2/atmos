#pragma once

#include "UniqueProviderSystem.h"
#include "FileManager.h"

namespace Atmos
{
    class FileSystem : public UniqueProviderSystem<FileManager>
    {
    public:
        FileSystem(ObjectManager& manager);
    private:
        void InitializeImpl() override;
    private:
        void CreateDefaultDirectories();
        void SetupFilePaths();
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::FileSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::FileSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}