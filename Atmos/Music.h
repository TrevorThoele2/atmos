#pragma once

#include "Object.h"
#include "ObjectReference.h"
#include "ObjectScribe.h"

#include "AudioAssetInstance.h"

#include "StoredProperty.h"

namespace Atmos::Audio
{
    class Music : public Object
    {
    public:
        typedef TypedObjectReference<Asset::AudioAssetInstance> AssetReference;
        typedef StoredProperty<AssetReference> AssetProperty;
        AssetProperty audioAsset;
    public:
        Music(ObjectManager& manager);
        Music(const ::Inscription::BinaryTableData<Music>& data);

        ObjectTypeDescription TypeDescription() const override;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Audio::Music> : ObjectTraitsBase<Audio::Music>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Audio::Music, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Audio::Music, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Audio::Music, BinaryArchive> :
        public ObjectScribe<::Atmos::Audio::Music, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}