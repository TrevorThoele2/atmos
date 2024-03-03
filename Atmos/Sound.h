
#pragma once

#include "Sense.h"

#include "AudioAssetInstance.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class Sound : public Sense
    {
    public:
        typedef TypedObjectReference<AudioAssetInstance> AssetReference;
        typedef StoredProperty<AssetReference> AssetProperty;
        AssetProperty audioAsset;
    public:
        Sound(ObjectManager& manager);
        Sound(const Sound& arg);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(Sound);

        ObjectTypeDescription TypeDescription() const override;
    private:
        void SubscribeToProperties();
    private:
        void OnEnabledChanged(bool newValue);
    };

    template<>
    struct ObjectTraits<Sound> : ObjectTraitsBase<Sound>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Sense> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Sound)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}