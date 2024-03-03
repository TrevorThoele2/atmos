
#pragma once

#include "Asset.h"

#include "ReadonlyProperty.h"
#include "StoredReadonlyProperty.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class ImageAssetData;

    class ImageAsset : public nFileAsset
    {
    public:
        typedef int Dimension;
    public:
        typedef StoredReadonlyProperty<Dimension> DimensionProperty;
        DimensionProperty width;
        DimensionProperty height;
    public:
        typedef ImageAssetData DataT;
        typedef std::unique_ptr<DataT> DataPtr;
    public:
        ImageAsset(const FileName& fileName, DataPtr&& data);
        ImageAsset(const ImageAsset& arg);
        ImageAsset(const ::Inscription::Table<ImageAsset>& table);

        DataT* Data();
        const DataT* Data() const;
        template<class RealDataT>
        RealDataT* DataAs();
        template<class RealDataT>
        const RealDataT* DataAs() const;

        ObjectTypeDescription TypeDescription() const override;
    private:
        DataPtr data;
    };

    template<class RealDataT>
    RealDataT* ImageAsset::DataAs()
    {
        return static_cast<RealDataT*>(data.get());
    }

    template<class RealDataT>
    const RealDataT* ImageAsset::DataAs() const
    {
        return static_cast<RealDataT*>(data.get());
    }

    template<>
    struct ObjectTraits<ImageAsset> : ObjectTraitsBase<ImageAsset>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<nFileAsset> bases = {};
    };

    class ImageAssetData
    {
    public:
        virtual ~ImageAssetData() = 0;

        virtual std::unique_ptr<ImageAssetData> Clone() const = 0;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::ImageAsset)
    {
    public:
        static void AddMembers(TableT& table);
    };
}