
#include "InitializationFileSystem.h"

#include "ObjectManager.h"
#include "FileSystem.h"

#include "StringUtility.h"
#include "FileUtility.h"
#include <Inscription/OutputTextFile.h>
#include <Inscription/InputTextFile.h>

namespace Atmos
{
    InitializationFileSystem::InitializationFileSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(InitializationFileSystem) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
    {}

    void InitializationFileSystem::Save()
    {
        FilePath filePath = CreateFilePath();
        ::Inscription::OutputTextFile file(filePath);
        file.ClearFile();

        file.WriteData(graphics.FileString());
        file.WriteData(sound.FileString());
        file.WriteData(controls.FileString());
    }

    FilePath InitializationFileSystem::CreateFilePath() const
    {
        return fileSystem->Get()->ExePath().Append(fileName).GetValue();
    }

    void InitializationFileSystem::InitializeImpl()
    {
        Load();
    }

    void InitializationFileSystem::Load()
    {
        FilePath filePath = CreateFilePath();
        if (!DoesFileExist(filePath))
        {
            Save();
            return;
        }

        ::Inscription::InputTextFile file(filePath);

        auto sectionList = FileSectionsAsList();

        FileSection* currentSection = nullptr;
        String line;
        FileSection::ExtractedSection extraction;

        // Returns true if we're starting a new section
        auto checkStartingNewSection = [line, &sectionList, &currentSection, &extraction]()
        {
            for (auto& checkSection : sectionList)
            {
                if (line != checkSection->Header())
                    continue;

                currentSection->SetTo(extraction);
                currentSection = checkSection;
                extraction = FileSection::ExtractedSection();
                extraction.header = line;
                return true;
            }

            return false;
        };

        while (!file.IsAtEndOfFile())
        {
            line = file.ReadLine();
            if (IsAllWhitespace(line))
                continue;

            if (extraction.IsOccupied() && checkStartingNewSection())
                continue;

            auto assignmentPosition = line.find_first_of(assignmentToken);
            auto name = line.substr(0, assignmentPosition);
            auto value = line.substr(assignmentPosition + 1);
            extraction.nameValues.emplace(name, value);
        }
    }

    InitializationFileSystem::FileSectionList InitializationFileSystem::FileSectionsAsList()
    {
        FileSectionList list;
        list.push_back(&graphics);
        list.push_back(&sound);
        list.push_back(&controls);
        return list;
    }

    void InitializationFileSystem::FindAndSetFileSystem()
    {
        fileSystem = Manager()->FindSystem<FileSystem>();
    }

    const String InitializationFileSystem::assignmentToken = "=";
}

namespace Inscription
{
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::InitializationFileSystem)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::InitializationFileSystem, "InitializationFileSystem");
}