
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

    Sound::Sound(const ::Inscription::Table<Sound>& table) : INSCRIPTION_TABLE_GET_BASE(Sense)
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

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Sound)
    {

    }
}