#pragma once

#include "FileAsset.h"

#include "ReadonlyProperty.h"
#include "StoredReadonlyProperty.h"

#include "ObjectScribe.h"

namespace Atmos::Asset
{
    class ImageAssetData;

    class ImageAsset : public FileAsset
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
        ImageAsset(ObjectManager& manager, const File::Name& fileName, DataPtr&& data);
        ImageAsset(const ImageAsset& arg);
        ImageAsset(const ::Inscription::BinaryTableData<ImageAsset>& data);

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

    class ImageAssetData
    {
    public:
        virtual ~ImageAssetData() = 0;

        virtual std::unique_ptr<ImageAssetData> Clone() const = 0;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Asset::ImageAsset> : public ObjectTraitsBase<Asset::ImageAsset>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Asset::FileAsset> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Asset::ImageAsset, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Asset::ImageAsset, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Asset::ImageAsset, BinaryArchive> :
        public ObjectScribe<::Atmos::Asset::ImageAsset, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}