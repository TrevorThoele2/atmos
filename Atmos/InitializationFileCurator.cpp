#include "InitializationFileCurator.h"

#include "InitializationInformation.h"

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

        Load();
    }

    void FileCurator::Save()
    {
        const auto filePath = FilePath();
        Inscription::File::OutputText file(filePath);
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

        Inscription::File::InputText file(filePath);

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

        while (!file.IsAtEnd())
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