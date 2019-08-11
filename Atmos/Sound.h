#pragma once

#include "AxisAlignedObject.h"

#include "AudioAssetInstance.h"

#include "ObjectScribe.h"

namespace Atmos::Audio
{
    class Sound : public AxisAlignedObject
    {
    public:
        StoredProperty<bool> enabled;
    public:
        typedef TypedObjectReference<Asset::AudioAssetInstance> AssetReference;
        typedef StoredProperty<AssetReference> AssetProperty;
        AssetProperty audioAsset;
    public:
        Sound(ObjectManager& manager);
        Sound(const Sound& arg);
        Sound(const ::Inscription::BinaryTableData<Sound>& data);

        ObjectTypeDescription TypeDescription() const override;
    private:
        void SubscribeToProperties();
    private:
        void OnEnabledChanged(bool newValue);
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Audio::Sound> : ObjectTraitsBase<Audio::Sound>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<AxisAlignedObject> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Audio::Sound, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Audio::Sound, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Audio::Sound, BinaryArchive> :
        public ObjectScribe<::Atmos::Audio::Sound, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}