#pragma once

#include <vector>

#include "ObjectSystem.h"

#include "InitializationFileSection.h"

#include "FilePath.h"
#include "FileName.h"

namespace Atmos
{
    namespace File
    {
        class FileSystem;
    }

    class InitializationFileSystem : public ObjectSystem
    {
    public:
        typedef InitializationFileSection FileSection;
    public:
        FileSection graphics = FileSection("<GRAPHICS>");
        FileSection sound = FileSection("<SOUND>");
        FileSection controls = FileSection("<CONTROLS>");
    public:
        InitializationFileSystem(ObjectManager& manager);

        void Save();
    protected:
        void InitializeImpl() override;
    private:
        const File::Name fileName = File::Name("Initialization.ini");
        File::Path CreateFilePath() const;
    private:
        void Load();
    private:
        typedef std::vector<FileSection*> FileSectionList;
        FileSectionList FileSectionsAsList();
    private:
        File::FileSystem* fileSystem;
        void FindAndSetFileSystem();
    private:
        static const String assignmentToken;
    };

    template<>
    struct ObjectSystemTraits<InitializationFileSystem>
    {
        static const ObjectSystemPriority priority = std::numeric_limits<ObjectSystemPriority>::max();
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::InitializationFileSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::InitializationFileSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}