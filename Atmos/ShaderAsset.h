
#pragma once

#include "Asset.h"

#include "ReadonlyProperty.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class ShaderAssetData;

    class ShaderAsset : public FileAsset
    {
    public:
        typedef ShaderAssetData DataT;
        typedef std::unique_ptr<DataT> DataPtr;
    public:
        ShaderAsset(ObjectManager& manager, const FileName& fileName, DataPtr&& data);
        ShaderAsset(const ShaderAsset& arg);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(ShaderAsset);

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

    template<>
    struct ObjectTraits<ShaderAsset> : ObjectTraitsBase<ShaderAsset>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<FileAsset> bases = {};
    };

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

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::ShaderAsset)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}