#pragma once

#include "Object.h"

#include "FileAsset.h"

#include "ReadonlyProperty.h"

#include "ObjectScribe.h"

namespace Atmos::Audio
{
    class AudioSystem;
}

namespace Atmos::Asset
{
    class AudioAssetData;
    class AssetPackageSystem;

    class AudioAsset : public FileAsset
    {
    public:
        typedef AudioAssetData DataT;
        typedef std::unique_ptr<DataT> DataPtr;
    public:
        AudioAsset(ObjectManager& manager, const File::Name& fileName, DataPtr&& data);
        AudioAsset(const AudioAsset& arg);
        AudioAsset(const ::Inscription::BinaryTableData<AudioAsset>& data);

        DataT* Data();
        const DataT* Data() const;
        template<class RealDataT>
        RealDataT* DataAs();
        template<class RealDataT>
        const RealDataT* DataAs() const;

        ObjectTypeDescription TypeDescription() const override;
    private:
        DataPtr data;
       
        void SetDataFromPackage(const File::Name& fileName);
    private:
        AssetPackageSystem* FindAssetPackageSystem();
        Audio::AudioSystem* FindAudioSystem();
    };

    template<class RealDataT>
    RealDataT* AudioAsset::DataAs()
    {
        return static_cast<RealDataT*>(data.get());
    }

    template<class RealDataT>
    const RealDataT* AudioAsset::DataAs() const
    {
        return static_cast<RealDataT*>(data.get());
    }

    class AudioAssetInstanceData;

    class AudioAssetData
    {
    public:
        virtual ~AudioAssetData() = 0;

        virtual std::unique_ptr<AudioAssetData> Clone() const = 0;

        virtual std::unique_ptr<AudioAssetInstanceData> CreateInstanceData() const = 0;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Asset::AudioAsset> : ObjectTraitsBase<Asset::AudioAsset>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Asset::FileAsset> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Asset::AudioAsset, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Asset::AudioAsset, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Asset::AudioAsset, BinaryArchive> :
        public ObjectScribe<::Atmos::Asset::AudioAsset, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}