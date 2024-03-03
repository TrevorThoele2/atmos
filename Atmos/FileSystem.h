#pragma once

#include "UniqueProviderSystem.h"
#include "FileManager.h"

namespace Atmos::File
{
    class FileSystem : public UniqueProviderSystem<FileManager>
    {
    public:
        FileSystem(ObjectManager& manager);
    protected:
        void InitializeImpl() override;
    private:
        void CreateDefaultDirectories();
        void SetupFilePaths();
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::File::FileSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::File::FileSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}