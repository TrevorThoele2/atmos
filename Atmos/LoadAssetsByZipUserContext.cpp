#include "LoadAssetsByZipUserContext.h"

namespace Inscription
{
    LoadAssetsByZipUserContext::LoadAssetsByZipUserContext(
        const Atmos::File::Path& filePath, Atmos::Logging::Logger& logger)
        :
        file(filePath, logger)
    {}

    auto LoadAssetsByZipUserContext::LoadAudioData(const Atmos::Name& name) -> std::optional<Extracted>
    {
        return Extract(name, &File::AllToExtract::audio, &File::Extracted::audio);
    }

    auto LoadAssetsByZipUserContext::LoadImageData(const Atmos::String& name) -> std::optional<Extracted>
    {
        return Extract(name, &File::AllToExtract::images, &File::Extracted::images);
    }

    auto LoadAssetsByZipUserContext::LoadShaderData(const Atmos::String& name) -> std::optional<Extracted>
    {
        return Extract(name, &File::AllToExtract::shaders, &File::Extracted::shaders);
    }

    auto LoadAssetsByZipUserContext::LoadFontData(const Atmos::Name& name) -> std::optional<Extracted>
    {
        return Extract(name, &File::AllToExtract::fonts, &File::Extracted::fonts);
    }

    auto LoadAssetsByZipUserContext::LoadScriptData(const Atmos::Name& name) -> std::optional<Extracted>
    {
        return Extract(name, &File::AllToExtract::scripts, &File::Extracted::scripts);
    }

    auto LoadAssetsByZipUserContext::Extract(
        const Atmos::Name& name,
        ToExtract toExtract,
        ExtractedFromFile extractedFromFile)

        -> std::optional<Extracted>
    {
        auto allToExtract = File::AllToExtract{};
        (allToExtract.*toExtract).emplace(name);
        auto extracted = file.ExtractAssets(allToExtract);

        if ((extracted.*extractedFromFile).empty())
            return {};
        else
        {
            auto& item = (extracted.*extractedFromFile)[0];
            return Extracted{ item.name, std::move(item.memory) };
        }
    }
}