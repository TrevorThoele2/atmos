#pragma once

#include "Object.h"

#include "Asset.h"

#include "ReadonlyProperty.h"

#include "ObjectScribe.h"

namespace Atmos
{
    class AudioAssetData;

    class AssetPackageSystem;
    class AudioSystem;

    class AudioAsset : public FileAsset
    {
    public:
        typedef AudioAssetData DataT;
        typedef std::unique_ptr<DataT> DataPtr;
    public:
        AudioAsset(ObjectManager& manager, const FileName& fileName, DataPtr&& data);
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
       
        void SetDataFromPackage(const FileName& fileName);
    private:
        AssetPackageSystem* FindAssetPackageSystem();
        AudioSystem* FindAudioSystem();
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

    template<>
    struct ObjectTraits<AudioAsset> : ObjectTraitsBase<AudioAsset>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<FileAsset> bases = {};
    };

    class AudioAssetInstanceData;

    class AudioAssetData
    {
    public:
        virtual ~AudioAssetData() = 0;

        virtual std::unique_ptr<AudioAssetData> Clone() const = 0;

        virtual std::unique_ptr<AudioAssetInstanceData> CreateInstanceData() const = 0;
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::AudioAsset, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::AudioAsset, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::AudioAsset, BinaryArchive> : public ObjectScribe<::Atmos::AudioAsset, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}