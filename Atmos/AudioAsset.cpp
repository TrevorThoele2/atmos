#include "AudioAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    AudioAsset::AudioAsset(Init init) : FileAsset(init)
    {}

    AudioAsset::AudioAsset(Init init, const ::Atmos::Name & name, DataPtr && data) :
        FileAsset(init, name, std::move(data))
    {}

    AudioAsset::AudioAsset(AudioAsset&& arg) noexcept : FileAsset(std::move(arg))
    {}

    AudioAsset& AudioAsset::operator=(AudioAsset&& arg) noexcept
    {
        FileAsset::operator=(std::move(arg));
        return *this;
    }
}

namespace Arca
{
    bool Traits<::Atmos::Asset::AudioAsset>::ShouldCreate(
        Reliquary& reliquary, const ::Atmos::Name& name, const ::Atmos::Asset::AudioAsset::DataPtr& data)
    {
        return Atmos::Asset::ShouldCreate<::Atmos::Asset::AudioAsset>(reliquary, name);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Asset::AudioAsset, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<Atmos::Asset::FileAsset<Atmos::Asset::AudioAssetData, Atmos::Asset::AudioAsset>>(
            object, archive);
    }
}