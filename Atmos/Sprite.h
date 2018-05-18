
#pragma once

#include <vector>

#include "RenderFragment.h"
#include "Material.h"
#include "MaterialRegistry.h"
#include "Color.h"

#include "AxisBoundingBox2D.h"

#include "FilePath.h"

#include "Serialization.h"
#include <AGUI\SpriteComponent.h>

namespace Atmos
{
    class Sprite : public RenderFragment
    {
    public:
        typedef unsigned short Index;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        AssetReference<Material> material;
        AssetReference<ShaderAsset> shaderPatch;

        AxisBoundingBox2D primaryAssetSlice;
        Index index;

        void Setup(Index index);
        void CalcPrimaryRect();

        void DrawImpl() const override;
        AxisBoundingBox3D::Coordinate GetZHeight() const override;
    public:
        Color color;

        Sprite();
        Sprite(const Name &materialName, Index index, const Color &color);
        Sprite(const Material &material, Index index, const Color &color);
        Sprite(const Sprite &arg);
        Sprite(Sprite &&arg);
        Sprite& operator=(const Sprite &arg);
        Sprite& operator=(Sprite &&arg);

        bool operator==(const Sprite &arg) const;
        bool operator!=(const Sprite &arg) const;

        void SetMaterial(const Name &name);
        void SetMaterial(const Material &set);
        void SetMaterial(const AssetReference<Material> &set);
        AssetReference<Material> GetMaterial() const;

        void SetShaderPatch(const Name &name);
        void SetShaderPatch(const ShaderAsset &set);
        void SetShaderPatch(const AssetReference<ShaderAsset> &set);
        AssetReference<ShaderAsset> GetShaderPatch() const;

        void SetIndex(Index setTo);
        void IncrementIndex(Index delta);
        Index GetIndex() const;
        // The primary image slice is the one viewed during normal gameplay
        const AxisBoundingBox2D& GetPrimaryAssetSlice() const;

        void SetXScaler(Size3D::ValueT set);
        void SetYScaler(Size3D::ValueT set);
        void SetZScaler(Size3D::ValueT set);
        void SetScalers(const Join3<Size3D::ValueT> &set);

        void SetColor(const Color &set);

        friend std::unique_ptr<agui::SpriteComponent> CreateAguiSpriteComponent(const Sprite &sprite, const agui::FileName &imageName, const agui::RelativePosition &relPosition);
        friend Sprite FromAguiSpriteComponent(const agui::SpriteComponent &sprite);
    };

    std::unique_ptr<agui::SpriteComponent> CreateAguiSpriteComponent(const Sprite &sprite, const agui::FileName &imageName, const agui::RelativePosition &relPosition = agui::RelativePosition());
    Sprite FromAguiSpriteComponent(const agui::SpriteComponent &sprite);
}