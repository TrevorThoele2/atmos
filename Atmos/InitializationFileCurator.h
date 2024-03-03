#pragma once

#include <Arca/Curator.h>

#include "InitializationFileSection.h"

#include <vector>

#include "FilePath.h"

namespace Atmos::Initialization
{
    class FileCurator final : public Arca::Curator
    {
    public:
        explicit FileCurator(Init init);

        void Save();
    private:
        FileSection graphics = FileSection("<GRAPHICS>");
        FileSection sound = FileSection("<SOUND>");
        FileSection controls = FileSection("<CONTROLS>");

        using FileSectionList = std::vector<FileSection*>;
        FileSectionList sectionList { &graphics, &sound, &controls};
    private:
        const File::Path fileName = File::Path("Initialization.ini");
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
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Initialization::FileCurator";
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Initialization::FileCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Initialization::FileCurator>;
    };
}