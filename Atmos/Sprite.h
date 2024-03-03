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
    class Sprite : public RenderFragment
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
        typedef ReadonlyProperty<AxisAlignedBox2D> AABBProperty;
        AABBProperty primaryAssetSlice;
    public:
        Sprite(ObjectManager& manager);
        Sprite(const Sprite& arg);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(Sprite);

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
        static constexpr ObjectTypeList<RenderFragment> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Sprite)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}