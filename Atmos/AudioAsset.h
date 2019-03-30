#pragma once

#include "Object.h"

#include "Asset.h"

#include "ReadonlyProperty.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class AudioAssetData;

    class AudioAsset : public nFileAsset
    {
    public:
        typedef AudioAssetData DataT;
        typedef std::unique_ptr<DataT> DataPtr;
    public:
        AudioAsset(const FileName& fileName, DataPtr&& data);
        AudioAsset(const AudioAsset& arg);
        AudioAsset(const ::Inscription::Table<AudioAsset>& table);

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
        static constexpr ObjectTypeList<nFileAsset> bases = {};
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
    DECLARE_OBJECT_INSCRIPTER(::Atmos::AudioAsset)
    {
    public:
        static void AddMembers(TableT& table);
    };
}