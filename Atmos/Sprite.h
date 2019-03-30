
#pragma once

#include "RenderFragment.h"
#include "Material.h"
#include "Color.h"
#include "AxisBoundingBox2D.h"

#include "ObjectReference.h"
#include "ObjectManager.h"
#include "nModulatorLinkMap.h"

#include "StoredProperty.h"

#include "FilePath.h"

#include "Serialization.h"
#include <AGUI\SpriteComponent.h>

namespace Atmos
{
    class nSprite : public nRenderFragment
    {
    public:
        typedef TypedObjectReference<Material> MaterialReference;
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
        nSprite();
        nSprite(const nSprite& arg);
        nSprite(const ::Inscription::Table<nSprite>& table);

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
    struct ObjectTraits<nSprite> : ObjectTraitsBase<nSprite>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<nRenderFragment> bases = {};
    };

    namespace Modulator
    {
        template<>
        struct LinkGeneratorGroup<nSprite> : public LinkGeneratorGroupBase<nSprite>
        {
            static Generator<Managed::Index> index;
            static Generator<Color::ValueT> colorA;
            static Generator<Color::ValueT> colorR;
            static Generator<Color::ValueT> colorG;
            static Generator<Color::ValueT> colorB;

            LinkGeneratorGroup();
        };
    }

    std::unique_ptr<Agui::SpriteComponent> CreateAguiSpriteComponent(
        const TypedObjectReference<nSprite> sprite,
        const Agui::FileName& imageName,
        const Agui::RelativePosition& relPosition = Agui::RelativePosition());
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::nSprite)
    {
    public:
        static void AddMembers(TableT& table);
    };
}