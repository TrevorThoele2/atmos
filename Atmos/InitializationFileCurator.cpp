#include "InitializationFileCurator.h"

#include "InitializationInformation.h"

#include "FileManagerProvider.h"

#include <Arca/Reliquary.h>

#include "StringUtility.h"
#include "FileUtility.h"
#include <Inscription/OutputTextFile.h>
#include <Inscription/InputTextFile.h>

namespace Atmos::Initialization
{
    void FileCurator::Save()
    {
        const auto filePath = FilePath();
        ::Inscription::OutputTextFile file(filePath);
        file.ClearFile();

        file.WriteData(graphics.FileString());
        file.WriteData(sound.FileString());
        file.WriteData(controls.FileString());
    }

    void FileCurator::InitializeImplementation()
    {
        information = Owner().Find<Information>();

        sound.CreateEntry("Master Volume", &information->sound.masterVolume);

        controls.CreateEntry("Move Up", &information->controls.actions.moveUp);
        controls.CreateEntry("Move Right", &information->controls.actions.moveRight);
        controls.CreateEntry("Move Down", &information->controls.actions.moveDown);
        controls.CreateEntry("Move Left", &information->controls.actions.moveLeft);
        controls.CreateEntry("Use", &information->controls.actions.use);
        controls.CreateEntry("Menu Up", &information->controls.actions.menuUp);
        controls.CreateEntry("Menu Right", &information->controls.actions.menuRight);
        controls.CreateEntry("Menu Down", &information->controls.actions.menuDown);
        controls.CreateEntry("Menu Left", &information->controls.actions.menuLeft);
        controls.CreateEntry("Select Next Character", &information->controls.actions.selectNextCharacter);
        controls.CreateEntry("Select Previous Character", &information->controls.actions.selectPreviousCharacter);
        controls.CreateEntry("End Turn", &information->controls.actions.endTurn);
        controls.CreateEntry("Attack", &information->controls.actions.attack);
        controls.CreateEntry("Cancel", &information->controls.actions.cancel);
        controls.CreateEntry("Inventory", &information->controls.actions.inventory);
        controls.CreateEntry("Spells", &information->controls.actions.spells);
        controls.CreateEntry("Stats", &information->controls.actions.stats);

        Load();
    }

    File::Path FileCurator::FilePath() const
    {
        return File::manager->ExePath() + fileName;
    }

    void FileCurator::Load()
    {
        File::Path filePath = FilePath();
        if (!File::DoesFileExist(filePath))
        {
            Save();
            return;
        }

        ::Inscription::InputTextFile file(filePath);

        FileSection* currentSection = nullptr;
        String line;
        FileSection::ExtractedSection extraction;

        // Returns true if we're starting a new section
        auto checkStartingNewSection = [this, line, &currentSection, &extraction]()
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
}

namespace Arca
{
    const TypeName Traits<Atmos::Initialization::FileCurator>::typeName = "InitializationFileCurator";
}