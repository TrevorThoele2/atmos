#pragma once

#include "RenderFragment.h"
#include "MaterialAsset.h"
#include "Color.h"
#include "AxisAlignedBox2D.h"

#include "ObjectReference.h"
#include "ObjectManager.h"

#include "StoredProperty.h"

#include "FilePath.h"

#include "Serialization.h"

namespace Atmos
{
    class Sprite : public Fragment
    {
    public:
        typedef TypedObjectReference<MaterialAsset> MaterialReference;
        typedef StoredProperty<MaterialReference> MaterialProperty;
        MaterialProperty material;
    public:
        typedef TypedObjectReference<ShaderAsset> ShaderReference;
        typedef StoredProperty<ShaderReference> ShaderProperty;
        ShaderProperty patchShader;
    public:
        typedef int Index;
        typedef StoredProperty<Index> IndexProperty;
        IndexProperty index;
    public:
        typedef StoredProperty<Color, Color&> ColorProperty;
        ColorProperty color;
    public:
        typedef ReadonlyProperty<AxisAlignedBox2D> BoxProperty;
        BoxProperty primaryAssetSlice = BoxProperty([this]() { return _primaryAssetSlice; });
    public:
        Sprite(ObjectManager& manager);
        Sprite(const Sprite& arg);
        Sprite(const ::Inscription::BinaryTableData<Sprite>& data);

        ObjectTypeDescription TypeDescription() const override;
    private:
        void DrawImpl() override;
    private:
        void SubscribeToProperties();
    private:
        AxisAlignedBox2D _primaryAssetSlice;
        void CalculatePrimaryAssetSlice();
    private:
        void OnPositionChanged(Position3D newValue);
        void OnMaterialChanged(MaterialReference newValue);
        void OnIndexChanged(Index newValue);
    };

    template<>
    struct ObjectTraits<Sprite> : ObjectTraitsBase<Sprite>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Fragment> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Sprite, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Sprite, BinaryArchive>
    {
        ObjectT::MaterialReference material;
        ObjectT::ShaderReference patchShader;
        ObjectT::Index index;
        ::Atmos::Color color;
    };

    template<>
    class Scribe<::Atmos::Sprite, BinaryArchive> :
        public ObjectScribe<::Atmos::Sprite, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        };
    };
}