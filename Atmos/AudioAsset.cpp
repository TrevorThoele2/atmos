
#include "AudioAsset.h"

#include "GameEnvironment.h"
#include "Environment.h"

namespace Atmos
{
    AudioAsset::AudioAsset(const FileName& fileName, DataPtr&& data) : nFileAsset(fileName), data(std::move(data))
    {}

    AudioAsset::AudioAsset(const AudioAsset& arg) : nFileAsset(arg), data((arg.data) ? arg.data->Clone() : nullptr)
    {}

    AudioAsset::AudioAsset(const ::Inscription::Table<AudioAsset>& table) : INSCRIPTION_TABLE_GET_BASE(nFileAsset)
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
        auto buffer = GameEnvironment::GetAssetPackage().RetrieveBuffer<AudioAsset>(fileName);
        data = std::move(Environment::GetAudio()->CreateAudioData(buffer.Get().first, buffer.Get().second, fileName));
    }

    const ObjectTypeName ObjectTraits<AudioAsset>::typeName = "AudioAsset";

    AudioAssetData::~AudioAssetData()
    {}
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::AudioAsset)
    {

    }
}