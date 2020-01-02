#pragma once

#include <vector>

#include <Arca/Curator.h>

#include "InitializationFileSection.h"

#include "FilePath.h"
#include "FileName.h"

namespace Atmos::Initialization
{
    class Information;

    class FileCurator final : public Arca::Curator
    {
    public:
        void Save();
    protected:
        void InitializeImplementation() override;
    private:
        Information* information = nullptr;
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
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Initialization::FileCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Initialization::FileCurator, BinaryArchive>
    {};
}