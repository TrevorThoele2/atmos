#include "LoadAssetsByZipUserContext.h"

namespace Inscription
{
    LoadAssetsByZipUserContext::LoadAssetsByZipUserContext(
        const Atmos::File::Path& filePath, Atmos::Logging::Logger& logger)
        :
        inputArchive(filePath, logger)
    {}

    auto LoadAssetsByZipUserContext::LoadAudioData(const Atmos::Name& name) -> std::optional<Extracted>
    {
        return Extract(name, &ArchiveInterface::AllToExtract::audio, &ArchiveInterface::Extracted::audio);
    }

    auto LoadAssetsByZipUserContext::LoadImageData(const Atmos::String& name) -> std::optional<Extracted>
    {
        return Extract(name, &ArchiveInterface::AllToExtract::images, &ArchiveInterface::Extracted::images);
    }

    auto LoadAssetsByZipUserContext::LoadShaderData(const Atmos::String& name) -> std::optional<Extracted>
    {
        return Extract(name, &ArchiveInterface::AllToExtract::shaders, &ArchiveInterface::Extracted::shaders);
    }

    auto LoadAssetsByZipUserContext::LoadScriptData(const Atmos::Name& name) -> std::optional<Extracted>
    {
        return Extract(name, &ArchiveInterface::AllToExtract::scripts, &ArchiveInterface::Extracted::scripts);
    }

    auto LoadAssetsByZipUserContext::Extract(
        const Atmos::Name& name,
        ToExtract toExtract,
        ExtractedFromArchive extractedFromAssets)

        -> std::optional<Extracted>
    {
        auto allToExtract = ArchiveInterface::AllToExtract{};
        (allToExtract.*toExtract).emplace(name);
        auto extracted = inputArchive.ExtractAssets(allToExtract);

        if ((extracted.*extractedFromAssets).empty())
            return {};
        else
        {
            auto& item = (extracted.*extractedFromAssets)[0];
            return Extracted{ item.name, std::move(item.memory) };
        }
    }
}