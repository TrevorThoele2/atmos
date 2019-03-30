
#include "Sound.h"

namespace Atmos
{
    nSound::nSound()
    {
        SubscribeToProperties();
    }

    nSound::nSound(const nSound& arg) : audioAsset(arg.audioAsset)
    {
        SubscribeToProperties();
    }

    nSound::nSound(const ::Inscription::Table<nSound>& table) : INSCRIPTION_TABLE_GET_BASE(nSense)
    {}

    ObjectTypeDescription nSound::TypeDescription() const
    {
        return ObjectTraits<nSound>::TypeDescription();
    }

    void nSound::SubscribeToProperties()
    {
        enabled.onValueChanged.Subscribe(&nSound::OnEnabledChanged, *this);
    }

    void nSound::OnEnabledChanged(bool newValue)
    {

    }

    const ObjectTypeName ObjectTraits<nSound>::typeName = "Sound";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::nSound)
    {

    }
}