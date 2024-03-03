#include "InputAssetsFile.h"

#include "Buffer.h"
#include <wx/mstream.h>

#include <Chroma/StringUtility.h>
#include "Logger.h"

namespace Atmos::World::Serialization
{
    InputAssetsFile::ExtractedAsset::ExtractedAsset(const Name& name, Buffer&& memory) :
        name(name), memory(std::move(memory))
    {}

    InputAssetsFile::AllToExtract::AllToExtract(
        const ToExtract& audio,
        const ToExtract& images,
        const ToExtract& shaders,
        const ToExtract& fonts,
        const ToExtract& scripts)
        :
        audio(audio), images(images), shaders(shaders), fonts(fonts), scripts(scripts)
    {}

    InputAssetsFile::InputAssetsFile(
        const File::Path& filePath, Logging::Logger& logger)
        :
        filePath(filePath),
        logger(&logger)
    {
        if (!std::filesystem::exists(filePath))
            return;

        try
        {
            stream = std::make_unique<wxFileInputStream>(filePath.c_str());
        }
        catch(...)
        { }
    }

    auto InputAssetsFile::ExtractAssets(const AllToExtract& toExtract) -> Extracted
    {
        Extracted extracted;

        if (!stream || !stream->IsOk())
        {
            logger->Log(
                "Could not open file to assets file.",
                Logging::Severity::Error,
                Logging::Details{ { "FilePath", filePath.string() } });
            return extracted;
        }

        struct ExtractionSet
        {
            String folderName;
            ToExtract AllToExtract::*toExtract;
            ExtractedAssets Extracted::* extractedAssets;
        };

        std::vector<ExtractionSet> extractionSets = {
            {"audio", &AllToExtract::audio, &Extracted::audio},
            {"images", &AllToExtract::images, &Extracted::images},
            {"shaders", &AllToExtract::shaders, &Extracted::shaders},
            {"fonts", &AllToExtract::fonts, &Extracted::fonts},
            {"scripts", &AllToExtract::scripts, &Extracted::scripts}
        };

        wxZipInputStream zip(*stream);

        std::unique_ptr<wxZipEntry> entry;
        while(entry.reset(zip.GetNextEntry()), entry)
        {
            auto name = entry->GetName().ToStdString();

            const auto splitName = Chroma::Split(name, String("\\"));
            if (splitName.size() < 2)
                continue;

            const auto folderName = splitName[splitName.size() - 2];
            const auto fileName = File::Path(splitName[splitName.size() - 1]).replace_extension().string();

            for(auto& extractionSet : extractionSets)
            {
                if (folderName != extractionSet.folderName)
                    continue;

                auto& toExtractFrom = toExtract.*extractionSet.toExtract;
                if (toExtractFrom.find(fileName) == toExtractFrom.end())
                    continue;

                (extracted.*extractionSet.extractedAssets).emplace_back(fileName, ExtractData(zip));
                break;
            }
        }

        return extracted;
    }

    auto InputAssetsFile::ExtractData(wxZipInputStream& zip) -> Buffer
    {
        wxMemoryOutputStream outputStream;

        zip.Read(outputStream);

        const auto size = outputStream.GetSize();
        auto buffer = Buffer();
        buffer.resize(size);

        outputStream.CopyTo(&buffer[0], size);

        return buffer;
    }
}