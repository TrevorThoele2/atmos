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

namespace Atmos::Render
{
    class Sprite : public RenderFragment
    {
    public:
        typedef TypedObjectReference<Asset::MaterialAsset> MaterialReference;
        typedef StoredProperty<MaterialReference> MaterialProperty;
        MaterialProperty material;
    public:
        typedef TypedObjectReference<Asset::ShaderAsset> ShaderReference;
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
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Render::Sprite> : ObjectTraitsBase<Render::Sprite>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Render::RenderFragment> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Render::Sprite, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Render::Sprite, BinaryArchive>
    {
        ObjectT::MaterialReference material;
        ObjectT::ShaderReference patchShader;
        ObjectT::Index index;
        ::Atmos::Render::Color color;
    };

    template<>
    class Scribe<::Atmos::Render::Sprite, BinaryArchive> :
        public ObjectScribe<::Atmos::Render::Sprite, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        };
    };
}