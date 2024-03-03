#include "AudioAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    AudioAsset::AudioAsset() = default;

    AudioAsset::AudioAsset(const ::Inscription::BinaryTableData<AudioAsset>& data) :
        FileAsset(data.base)
    {}

    AudioAsset::DataT* AudioAsset::Data()
    {
        return data.get();
    }

    const AudioAsset::DataT* AudioAsset::Data() const
    {
        return data.get();
    }

    void AudioAsset::Initialize(const File::Name& fileName, DataPtr&& data)
    {
        SetFileName(fileName);
        this->data = std::move(data);
    }

    AudioAssetData::~AudioAssetData() = default;
}

namespace Arca
{
    const TypeName Traits<::Atmos::Asset::AudioAsset>::typeName = "AudioAsset";

    bool Traits<::Atmos::Asset::AudioAsset>::ShouldCreate(
        Reliquary& reliquary, const ::Atmos::File::Name& fileName, ::Atmos::Asset::AudioAsset::DataPtr&& data)
    {
        return Atmos::Asset::ShouldCreateAsset<::Atmos::Asset::AudioAsset>(reliquary, fileName);
    }
}

namespace Inscription
{
    Scribe<::Atmos::Asset::AudioAsset, BinaryArchive>::Table::Table()
    {
        MergeDataLinks({
            DataLink::Base(data.base) }
        );
    }
}