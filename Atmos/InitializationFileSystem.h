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
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(InitializationFileSystem);

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
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::InitializationFileSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}