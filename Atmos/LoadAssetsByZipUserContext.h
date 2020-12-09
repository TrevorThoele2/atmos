#pragma once

#include "LoadAssetsUserContext.h"

#include "InputAssetsArchiveInterface.h"

#include "Logger.h"

namespace Inscription
{
    class LoadAssetsByZipUserContext final : public LoadAssetsUserContext
    {
    public:
        LoadAssetsByZipUserContext(const Atmos::File::Path& filePath, Atmos::Logging::Logger& logger);
    public:
        std::optional<Extracted> LoadImage(const Atmos::Name& name) override;
        std::optional<Extracted> LoadShader(const Atmos::Name& name) override;
        std::optional<Extracted> LoadScript(const Atmos::Name& name) override;
    private:
        using ArchiveInterface = Atmos::World::Serialization::InputAssetsArchiveInterface;
        ArchiveInterface inputArchive;

        using ToExtract =
            ArchiveInterface::ToExtract ArchiveInterface::AllToExtract::*;
        using ExtractedFromArchive =
            ArchiveInterface::ExtractedAssets ArchiveInterface::Extracted::*;

        std::optional<Extracted> Extract(
            const Atmos::Name& name,
            ToExtract toExtract,
            ExtractedFromArchive extractedFromAssets);
    };
}