#pragma once

#include "Sense.h"

#include "AudioAssetInstance.h"

#include "ObjectScribe.h"

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
        Sound(const ::Inscription::BinaryTableData<Sound>& data);

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
    template<>
    struct TableData<::Atmos::Sound, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Sound, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Sound, BinaryArchive> :
        public ObjectScribe<::Atmos::Sound, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}