#include "InitializationFileCurator.h"

#include "InitializationInformation.h"

#include "FileManagerProvider.h"

#include <Arca/Reliquary.h>

#include "StringUtility.h"
#include <Inscription/OutputTextFile.h>
#include <Inscription/InputTextFile.h>

namespace Atmos::Initialization
{
    FileCurator::FileCurator(Init init) : Curator(init)
    {
        auto mutableInformation = MutablePointer().Of<Information>();

        sound.CreateEntry("Master Volume", &mutableInformation->sound.masterVolume);

        controls.CreateEntry("Move Up", &mutableInformation->controls.actions.moveUp);
        controls.CreateEntry("Move Right", &mutableInformation->controls.actions.moveRight);
        controls.CreateEntry("Move Down", &mutableInformation->controls.actions.moveDown);
        controls.CreateEntry("Move Left", &mutableInformation->controls.actions.moveLeft);
        controls.CreateEntry("Use", &mutableInformation->controls.actions.use);
        controls.CreateEntry("Menu Up", &mutableInformation->controls.actions.menuUp);
        controls.CreateEntry("Menu Right", &mutableInformation->controls.actions.menuRight);
        controls.CreateEntry("Menu Down", &mutableInformation->controls.actions.menuDown);
        controls.CreateEntry("Menu Left", &mutableInformation->controls.actions.menuLeft);
        controls.CreateEntry("Select Next Character", &mutableInformation->controls.actions.selectNextCharacter);
        controls.CreateEntry("Select Previous Character", &mutableInformation->controls.actions.selectPreviousCharacter);
        controls.CreateEntry("End Turn", &mutableInformation->controls.actions.endTurn);
        controls.CreateEntry("Attack", &mutableInformation->controls.actions.attack);
        controls.CreateEntry("Cancel", &mutableInformation->controls.actions.cancel);
        controls.CreateEntry("Inventory", &mutableInformation->controls.actions.inventory);
        controls.CreateEntry("Spells", &mutableInformation->controls.actions.spells);
        controls.CreateEntry("Stats", &mutableInformation->controls.actions.stats);

        Load();
    }

    void FileCurator::Save()
    {
        const auto filePath = FilePath();
        Inscription::OutputTextFile file(filePath);
        file.ClearFile();

        file.WriteData(graphics.FileString());
        file.WriteData(sound.FileString());
        file.WriteData(controls.FileString());
    }

    File::Path FileCurator::FilePath() const
    {
        return std::filesystem::current_path() / fileName;
    }

    void FileCurator::Load()
    {
        File::Path filePath = FilePath();
        if (!std::filesystem::exists(filePath))
        {
            Save();
            return;
        }

        Inscription::InputTextFile file(filePath);

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