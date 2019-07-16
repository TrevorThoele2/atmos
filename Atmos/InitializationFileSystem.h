#pragma once

#include <vector>

#include "ObjectSystem.h"

#include "InitializationFileSection.h"

#include "FilePath.h"
#include "FileName.h"

namespace Atmos
{
    class FileSystem;

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
    private:
        const FileName fileName = FileName("Initialization.ini");
        FilePath CreateFilePath() const;
    private:
        void InitializeImpl() override;
    private:
        void Load();
    private:
        typedef std::vector<FileSection*> FileSectionList;
        FileSectionList FileSectionsAsList();
    private:
        FileSystem* fileSystem;
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
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}