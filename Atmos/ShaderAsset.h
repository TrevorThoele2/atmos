#pragma once

#include "FileAsset.h"

#include "ReadonlyProperty.h"

#include "ObjectScribe.h"

namespace Atmos::Asset
{
    class ShaderAssetData;

    class ShaderAsset : public FileAsset
    {
    public:
        typedef ShaderAssetData DataT;
        typedef std::unique_ptr<DataT> DataPtr;
    public:
        ShaderAsset(ObjectManager& manager, const File::Name& fileName, DataPtr&& data);
        ShaderAsset(const ShaderAsset& arg);
        ShaderAsset(const ::Inscription::BinaryTableData<ShaderAsset>& data);

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
    RealDataT* ShaderAsset::DataAs()
    {
        return static_cast<RealDataT*>(data.get());
    }

    template<class RealDataT>
    const RealDataT* ShaderAsset::DataAs() const
    {
        return static_cast<RealDataT*>(data.get());
    }

    class ShaderAssetData
    {
    public:
        typedef unsigned int PassT;
    public:
        virtual ~ShaderAssetData() = 0;

        virtual std::unique_ptr<ShaderAssetData> Clone() const = 0;
        
        virtual void Reset() = 0;
        virtual void Release() = 0;

        virtual PassT Begin() const = 0;
        virtual void End() const = 0;

        virtual void BeginNextPass(PassT pass) const = 0;
        virtual void EndPass() const = 0;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Asset::ShaderAsset> : ObjectTraitsBase<Asset::ShaderAsset>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Asset::FileAsset> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Asset::ShaderAsset, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Asset::ShaderAsset, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Asset::ShaderAsset, BinaryArchive> :
        public ObjectScribe<::Atmos::Asset::ShaderAsset, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}