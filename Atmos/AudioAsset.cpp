#include "AudioAsset.h"

#include "ObjectManager.h"
#include "AssetPackageSystem.h"
#include "AudioSystem.h"

namespace Atmos
{
    AudioAsset::AudioAsset(ObjectManager& manager, const FileName& fileName, DataPtr&& data) :
        FileAsset(manager, fileName), data(std::move(data))
    {}

    AudioAsset::AudioAsset(const AudioAsset& arg) : FileAsset(arg), data((arg.data) ? arg.data->Clone() : nullptr)
    {}

    AudioAsset::AudioAsset(const ::Inscription::BinaryTableData<AudioAsset>& data) :
        FileAsset(std::get<0>(data.bases))
    {
        SetDataFromPackage(fileName);
    }

    AudioAsset::DataT* AudioAsset::Data()
    {
        return data.get();
    }

    const AudioAsset::DataT* AudioAsset::Data() const
    {
        return data.get();
    }

    ObjectTypeDescription AudioAsset::TypeDescription() const
    {
        return ObjectTraits<AudioAsset>::TypeDescription();
    }

    void AudioAsset::SetDataFromPackage(const FileName& fileName)
    {
        auto assetPackageSystem = FindAssetPackageSystem();
        auto audioManager = FindAudioSystem()->Get();

        auto buffer = assetPackageSystem->RetrieveBuffer<AudioAsset>(fileName);
        data = std::move(audioManager->CreateAudioData(buffer.Get().first, buffer.Get().second, fileName));
    }

    AssetPackageSystem* AudioAsset::FindAssetPackageSystem()
    {
        return Manager()->FindSystem<AssetPackageSystem>();
    }

    AudioSystem* AudioAsset::FindAudioSystem()
    {
        return Manager()->FindSystem<AudioSystem>();
    }

    const ObjectTypeName ObjectTraits<AudioAsset>::typeName = "AudioAsset";

    AudioAssetData::~AudioAssetData()
    {}
}