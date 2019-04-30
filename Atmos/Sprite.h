#pragma once

#include "RenderFragment.h"
#include "MaterialAsset.h"
#include "Color.h"
#include "AxisBoundingBox2D.h"

#include "ObjectReference.h"
#include "ObjectManager.h"

#include "StoredProperty.h"

#include "FilePath.h"

#include "Serialization.h"
#include <AGUI/SpriteComponent.h>

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
        typedef ReadonlyProperty<AxisBoundingBox2D> AABBProperty;
        AABBProperty primaryAssetSlice;
    public:
        Sprite(ObjectManager& manager);
        Sprite(const Sprite& arg);
        Sprite(const ::Inscription::Table<Sprite>& table);

        ObjectTypeDescription TypeDescription() const override;
    private:
        void DrawImpl() override;
    private:
        void SubscribeToProperties();
    private:
        AxisBoundingBox2D _primaryAssetSlice;
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

    std::unique_ptr<Agui::SpriteComponent> CreateAguiSpriteComponent(
        const TypedObjectReference<Sprite> sprite,
        const Agui::FileName& imageName,
        const Agui::RelativePosition& relPosition = Agui::RelativePosition());
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Sprite)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}