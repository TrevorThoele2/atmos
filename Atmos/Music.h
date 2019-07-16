#pragma once

#include "Object.h"

#include "ObjectReference.h"

#include "AudioAssetInstance.h"

#include "StoredProperty.h"

#include "ObjectScribe.h"

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
    template<>
    struct TableData<::Atmos::Music, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Music, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Music, BinaryArchive> :
        public ObjectScribe<::Atmos::Music, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}