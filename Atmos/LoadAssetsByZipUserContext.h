#pragma once

#include "LoadAssetsUserContext.h"

#include "InputAssetsArchiveInterface.h"

namespace Inscription
{
    class LoadAssetsByZipUserContext final : public LoadAssetsUserContext
    {
    public:
        LoadAssetsByZipUserContext(const Atmos::File::Path& filePath);
    public:
        std::optional<Extracted> LoadImage(const Atmos::Name& name) override;
        std::optional<Extracted> LoadShader(const Atmos::Name& name) override;
    private:
        Atmos::World::Serialization::InputAssetsArchiveInterface inputArchive;
    };
}