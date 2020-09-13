#include "LoadAssetsByZipUserContext.h"

namespace Inscription
{
    LoadAssetsByZipUserContext::LoadAssetsByZipUserContext(const Atmos::File::Path& filePath) :
        inputArchive(filePath)
    {}

    auto LoadAssetsByZipUserContext::LoadImage(const Atmos::String& name) -> std::optional<Extracted>
    {
        auto toExtract = Atmos::World::Serialization::InputAssetsArchiveInterface::AllToExtract{};
        toExtract.images.emplace(name);
        auto extracted = inputArchive.ExtractAssets(toExtract);

        if (extracted.images.empty())
            return {};
        else
            return Extracted{ extracted.images[0].name, std::move(extracted.images[0].memory) };
    }

    auto LoadAssetsByZipUserContext::LoadShader(const Atmos::String& name) -> std::optional<Extracted>
    {
        auto toExtract = Atmos::World::Serialization::InputAssetsArchiveInterface::AllToExtract{};
        toExtract.shaders.emplace(name);
        auto extracted = inputArchive.ExtractAssets(toExtract);

        if (extracted.shaders.empty())
            return {};
        else
            return Extracted{ extracted.shaders[0].name, std::move(extracted.shaders[0].memory) };
    }
}