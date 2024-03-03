#pragma once

#include "LoadAssetsUserContext.h"

#include "InputAssetsFile.h"

#include "Logger.h"

namespace Inscription
{
    class LoadAssetsByZipUserContext final : public LoadAssetsUserContext
    {
    public:
        LoadAssetsByZipUserContext(const Atmos::File::Path& filePath, Atmos::Logging::Logger& logger);
    public:
        std::optional<Extracted> LoadAudioData(const Atmos::Name& name) override;
        std::optional<Extracted> LoadImageData(const Atmos::Name& name) override;
        std::optional<Extracted> LoadShaderData(const Atmos::Name& name) override;
        std::optional<Extracted> LoadFontData(const Atmos::Name& name) override;
        std::optional<Extracted> LoadScriptData(const Atmos::Name& name) override;
    private:
        using File = Atmos::World::Serialization::InputAssetsFile;
        File file;

        using ToExtract = File::ToExtract File::AllToExtract::*;
        using ExtractedFromFile = File::ExtractedAssets File::Extracted::*;

        std::optional<Extracted> Extract(
            const Atmos::Name& name,
            ToExtract toExtract,
            ExtractedFromFile extractedFromFile);
    };
}