#include "InputAssetsArchiveInterface.h"

#include "Buffer.h"
#include <wx/mstream.h>

#include <Chroma/StringUtility.h>
#include "Logger.h"

namespace Atmos::World::Serialization
{
    InputAssetsArchiveInterface::ExtractedAsset::ExtractedAsset(const Name& name, DataBuffer&& memory) :
        name(name), memory(std::move(memory))
    {}

    InputAssetsArchiveInterface::AllToExtract::AllToExtract(const ToExtract& images, const ToExtract& shaders) :
        images(images), shaders(shaders)
    {}

    InputAssetsArchiveInterface::InputAssetsArchiveInterface(const File::Path& filePath) :
        filePath(filePath)
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

    auto InputAssetsArchiveInterface::ExtractAssets(const AllToExtract& toExtract) -> Extracted
    {
        Extracted extracted;

        if (!stream || !stream->IsOk())
        {
            Logging::logger.Log(
                "Could not open file to assets file.",
                Logging::Severity::Error,
                Logging::Details{ { "FilePath", filePath.string() } });
            return extracted;
        }

        wxZipInputStream zip(*stream);

        std::unique_ptr<wxZipEntry> entry;
        while(entry.reset(zip.GetNextEntry()), entry)
        {
            auto name = entry->GetName().ToStdString();

            const auto splitName = Chroma::Split(name, "\\");
            if (splitName.size() < 2)
                continue;

            const auto folderName = splitName[splitName.size() - 2];
            const auto fileName = splitName[splitName.size() - 1];

            if (folderName == "images")
            {
                if (toExtract.images.find(fileName) == toExtract.images.end())
                    continue;

                extracted.images.emplace_back(fileName, ExtractImage(zip));
            }
            else if (folderName == "shaders")
            {
                if (toExtract.shaders.find(fileName) == toExtract.shaders.end())
                    continue;

                extracted.shaders.emplace_back(fileName, ExtractImage(zip));
            }
        }

        return extracted;
    }

    auto InputAssetsArchiveInterface::ExtractImage(wxZipInputStream& zip) -> DataBuffer
    {
        wxMemoryOutputStream outputStream;

        zip.Read(outputStream);

        const auto size = outputStream.GetSize();
        auto buffer = DataBuffer();
        buffer.resize(size);

        outputStream.CopyTo(&buffer[0], size);

        return buffer;
    }
}