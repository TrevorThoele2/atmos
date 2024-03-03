#pragma once

#include <vector>

#include <Arca/Curator.h>

#include "InitializationInformation.h"
#include "InitializationFileSection.h"

#include "FilePath.h"
#include "FileName.h"

namespace Atmos::Initialization
{
    class FileCurator final : public Arca::Curator
    {
    public:
        explicit FileCurator(Init init);

        void Save();
    private:
        Arca::Index<Information> information;
    private:
        FileSection graphics = FileSection("<GRAPHICS>");
        FileSection sound = FileSection("<SOUND>");
        FileSection controls = FileSection("<CONTROLS>");

        using FileSectionList = std::vector<FileSection*>;
        FileSectionList sectionList { &graphics, &sound, &controls};
    private:
        const File::Name fileName = File::Name("Initialization.ini");
        [[nodiscard]] File::Path FilePath() const;

        void Load();
    private:
        const String assignmentToken = "=";
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Initialization::FileCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "InitializationFileCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Initialization::FileCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Initialization::FileCurator, BinaryArchive>
    {};
}