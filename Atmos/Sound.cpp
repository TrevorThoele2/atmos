#include "Sound.h"

namespace Atmos::Audio
{
    Sound::Sound(ObjectManager& manager) : AxisAlignedObject(manager)
    {
        SubscribeToProperties();
    }

    Sound::Sound(const Sound& arg) : AxisAlignedObject(arg), audioAsset(arg.audioAsset)
    {
        SubscribeToProperties();
    }

    Sound::Sound(const ::Inscription::BinaryTableData<Sound>& data) :
        AxisAlignedObject(std::get<0>(data.bases))
    {}

    ObjectTypeDescription Sound::TypeDescription() const
    {
        return ObjectTraits<Sound>::TypeDescription();
    }

    void Sound::SubscribeToProperties()
    {
        enabled.onValueChanged.Subscribe(&Sound::OnEnabledChanged, *this);
    }

    void Sound::OnEnabledChanged(bool newValue)
    {

    }
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Audio::Sound>::typeName = "Sound";
}