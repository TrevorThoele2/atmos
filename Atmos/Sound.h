
#pragma once

#include "Sense.h"

#include "AudioAssetInstance.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class nSound : public nSense
    {
    public:
        typedef TypedObjectReference<AudioAssetInstance> AssetReference;
        typedef StoredProperty<AssetReference> AssetProperty;
        AssetProperty audioAsset;
    public:
        nSound();
        nSound(const nSound& arg);
        nSound(const ::Inscription::Table<nSound>& table);

        ObjectTypeDescription TypeDescription() const override;
    private:
        void SubscribeToProperties();
    private:
        void OnEnabledChanged(bool newValue);
    };

    template<>
    struct ObjectTraits<nSound> : ObjectTraitsBase<nSound>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<nSense> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::nSound)
    {
    public:
        static void AddMembers(TableT& table);
    };
}