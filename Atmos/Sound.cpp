
#include "Sound.h"

namespace Atmos
{
    Sound::Sound(ObjectManager& manager) : Sense(manager)
    {
        SubscribeToProperties();
    }

    Sound::Sound(const Sound& arg) : Sense(arg), audioAsset(arg.audioAsset)
    {
        SubscribeToProperties();
    }

    Sound::Sound(const ::Inscription::BinaryTableData<Sound>& data) :
        Sense(std::get<0>(data.bases))
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

    const ObjectTypeName ObjectTraits<Sound>::typeName = "Sound";
}