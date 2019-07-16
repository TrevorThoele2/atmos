#pragma once

#include "Asset.h"

#include "ObjectReference.h"

#include "ImageAsset.h"
#include "ShaderAsset.h"

#include "StoredProperty.h"
#include "StoredReadonlyProperty.h"

#include "ObjectScribe.h"

namespace Atmos
{
    class MaterialAsset : public Asset
    {
    public:
        typedef int Dimension;
        typedef int GridDimension;
    public:
        typedef TypedObjectReference<ImageAsset> ImageAssetReference;
        typedef TypedObjectReference<ShaderAsset> ShaderAssetReference;

        typedef StoredProperty<ImageAssetReference> ImageProperty;
        typedef StoredProperty<ShaderAssetReference> ShaderProperty;

        typedef ReadonlyProperty<Dimension> DimensionProperty;
        typedef StoredProperty<GridDimension> GridDimensionProperty;
    public:
        ImageProperty xVisual;
        ImageProperty yVisual;
        ImageProperty zVisual;
        ImageProperty xNormal;
        ImageProperty yNormal;
        ImageProperty zNormal;
        ImageProperty xHeight;
        ImageProperty yHeight;
        ImageProperty zHeight;

        ShaderProperty shader;

        DimensionProperty width = [this]() { return _width; };
        DimensionProperty height = [this]() { return _height; };

        GridDimensionProperty columns;
        GridDimensionProperty rows;
    public:
        MaterialAsset(ObjectManager& manager, const Name& name);
        MaterialAsset(const MaterialAsset& arg);
        MaterialAsset(const ::Inscription::BinaryTableData<MaterialAsset>& data);

        ObjectTypeDescription TypeDescription() const override;
    private:
        Dimension _width;
        Dimension _height;

        void CalculateDimensions();
    private:
        void DoOnImageProperties(std::function<void(ImageProperty&)>& func);
        void DoOnImageProperties(std::function<void(const ImageProperty&)>& func) const;

        void SubscribeToProperties();
        void OnImagePropertyChanged(TypedObjectReference<ImageAsset> reference);
    };

    template<>
    struct ObjectTraits<MaterialAsset> : ObjectTraitsBase<MaterialAsset>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Asset> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::MaterialAsset, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::MaterialAsset, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::MaterialAsset, BinaryArchive> : public ObjectScribe<::Atmos::MaterialAsset, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}