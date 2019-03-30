#pragma once

#include "Object.h"

#include "ObjectReference.h"

#include "AudioAssetInstance.h"

#include "StoredProperty.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class Music : public Object
    {
    public:
        typedef TypedObjectReference<AudioAssetInstance> AssetReference;
        typedef StoredProperty<AssetReference> AssetProperty;
        AssetProperty audioAsset;
    public:
        ObjectTypeDescription TypeDescription() const override;
    };

    template<>
    struct ObjectTraits<Music> : ObjectTraitsBase<Music>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Music)
    {
    public:
        static void AddMembers(TableT& table);
    };
}