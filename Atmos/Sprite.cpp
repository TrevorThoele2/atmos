
#include "Sprite.h"

#include "ShaderRegistry.h"

#include "Environment.h"
#include "CurrentField.h"

#include "AguiSpriteFactory.h"
#include <AGUI\ImageResource.h>

#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>
#include <Inscription\Table.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Sprite)
    {
        inscription::BaseSerialize<::Atmos::RenderFragment>(scribe, *this);
        scribe(material);
        scribe(shaderPatch);
        scribe(index);
        scribe(color);
        if (scribe.IsInput())
            CalcPrimaryRect();
    }

    void Sprite::Setup(Index index)
    {
        SetIndex(index);
        primaryAssetSlice.SetPixelPerfect(false);
    }

    void Sprite::CalcPrimaryRect()
    {
        if (!material || !material->GetXVisual())
        {
            primaryAssetSlice.SetTop(0);
            primaryAssetSlice.SetBottom(0);
            primaryAssetSlice.SetLeft(0);
            primaryAssetSlice.SetRight(0);

            SetWidth(0.0f);
            SetHeight(0.0f);
            SetDepth(GRID_SIZE<float>);
            return;
        }

        auto xVisual = material->GetXVisual();
        ImageAsset::GridDimension cols(xVisual->GetColumns());
        ImageAsset::GridDimension rows(xVisual->GetRows());

        auto indexWidth = static_cast<float>(xVisual->GetWidth() / cols);
        auto indexHeight = static_cast<float>(xVisual->GetHeight() / rows);

        auto col = index % cols;
        if (col == 0)
            col = cols;
        col--;

        auto row = static_cast<int>(std::ceil(static_cast<float>(index) / static_cast<float>(cols)));
        row--;

        primaryAssetSlice.SetTop(row * indexHeight);
        primaryAssetSlice.SetBottom(row * indexHeight + indexHeight);
        primaryAssetSlice.SetLeft(col * indexWidth);
        primaryAssetSlice.SetRight(col * indexWidth + indexWidth);

        // Calculate size
        SetWidth(static_cast<float>(xVisual->GetWidth() / xVisual->GetColumns()));
        SetHeight(static_cast<float>(xVisual->GetHeight() / xVisual->GetRows()));
        SetDepth(GRID_SIZE<float>);
    }

    void Sprite::DrawImpl() const
    {
        if (!material || !material->GetXVisual())
            return;

        Environment::GetGraphics()->RenderSprite(*this);
    }

    AxisBoundingBox3D::Coordinate Sprite::GetZHeight() const
    {
        return 0;
    }

    Sprite::Sprite() : RenderFragment(true), material(nullptr)
    {
        Setup(1);
    }

    Sprite::Sprite(const Name &materialName, Index index, const Color &color) : RenderFragment(true), material(MaterialRegistry::Find(materialName)), color(color)
    {
        Setup(index);
    }

    Sprite::Sprite(const Material &material, Index index, const Color &color) : RenderFragment(true), material(&material), color(color)
    {
        Setup(index);
    }

    Sprite::Sprite(const Sprite &arg) : RenderFragment(arg), material(arg.material), shaderPatch(arg.shaderPatch), primaryAssetSlice(arg.primaryAssetSlice), index(arg.index), color(arg.color)
    {}

    Sprite::Sprite(Sprite &&arg) : RenderFragment(std::move(arg)), material(arg.material), shaderPatch(std::move(arg.shaderPatch)), primaryAssetSlice(std::move(arg.primaryAssetSlice)), index(arg.index), color(std::move(arg.color))
    {}

    Sprite& Sprite::operator=(const Sprite &arg)
    {
        if (!IsDynamic())
            return *this;

        RenderFragment::operator=(arg);
        material = arg.material;
        shaderPatch = arg.shaderPatch;
        primaryAssetSlice = arg.primaryAssetSlice;
        index = arg.index;
        color = arg.color;
        return *this;
    }

    Sprite& Sprite::operator=(Sprite &&arg)
    {
        if (!IsDynamic())
            return *this;

        RenderFragment::operator=(std::move(arg));
        material = std::move(arg.material);
        shaderPatch = std::move(arg.shaderPatch);
        primaryAssetSlice = std::move(arg.primaryAssetSlice);
        index = std::move(arg.index);
        color = std::move(arg.color);
        return *this;
    }

    bool Sprite::operator==(const Sprite &arg) const
    {
        return RenderFragment::operator==(arg) && material == arg.material && shaderPatch == arg.shaderPatch && primaryAssetSlice == arg.primaryAssetSlice && index == arg.index && color == arg.color;
    }

    bool Sprite::operator!=(const Sprite &arg) const
    {
        return !(*this == arg);
    }

    void Sprite::SetMaterial(const Name &name)
    {
        SetMaterial(AssetRegistry<Material>::Find(name));
    }

    void Sprite::SetMaterial(const Material &set)
    {
        SetMaterial(AssetReference<Material>(set));
    }

    void Sprite::SetMaterial(const AssetReference<Material> &set)
    {
        if (!IsDynamic())
            return;

        material = set;
        CalcPrimaryRect();
    }

    AssetReference<Material> Sprite::GetMaterial() const
    {
        return material;
    }

    void Sprite::SetShaderPatch(const Name &name)
    {
        SetShaderPatch(ShaderRegistry::Find(name));
    }

    void Sprite::SetShaderPatch(const ShaderAsset &set)
    {
        SetShaderPatch(AssetReference<ShaderAsset>(set));
    }

    void Sprite::SetShaderPatch(const AssetReference<ShaderAsset> &set)
    {
        shaderPatch = set;
    }

    AssetReference<ShaderAsset> Sprite::GetShaderPatch() const
    {
        return shaderPatch;
    }

    void Sprite::SetIndex(Index setTo)
    {
        if (IsStatic())
            return;

        if (setTo == 0)
            index = 1;
        else
        {
            if (!material || !material->GetXVisual() || setTo <= material->GetXVisual()->GetColumns() * material->GetXVisual()->GetRows())
                index = setTo;
            else
                index = material->GetXVisual()->GetColumns() * material->GetXVisual()->GetRows();
        }

        CalcPrimaryRect();
    }

    void Sprite::IncrementIndex(Index delta)
    {
        if (IsStatic())
            return;

        index += delta;
        CalcPrimaryRect();
    }

    Sprite::Index Sprite::GetIndex() const
    {
        return index;
    }

    const AxisBoundingBox2D& Sprite::GetPrimaryAssetSlice() const
    {
        return primaryAssetSlice;
    }

    void Sprite::SetXScaler(Size3D::ValueT set)
    {
        RenderFragment::SetXScaler(set);
    }

    void Sprite::SetYScaler(Size3D::ValueT set)
    {
        RenderFragment::SetYScaler(set);
    }

    void Sprite::SetZScaler(Size3D::ValueT set)
    {
        RenderFragment::SetZScaler(set);
    }

    void Sprite::SetScalers(const Join3<Size3D::ValueT> &set)
    {
        RenderFragment::SetScalers(set);
    }

    void Sprite::SetColor(const Color &set)
    {
        color = set;
    }

    // Returns heap memory. Make sure to delete it if not giving it to AGUI
    std::unique_ptr<agui::SpriteComponent> CreateAguiSpriteComponent(const Sprite &sprite, const agui::FileName &imageName, const agui::RelativePosition &relPosition)
    {
        if (!sprite.GetMaterial() || !sprite.GetMaterial()->GetXVisual())
            return std::unique_ptr<agui::SpriteComponent>();

        AguiSpriteFactory factory(sprite);
        return std::unique_ptr<agui::SpriteComponent>(factory.CreateComponent(imageName, relPosition));
    }

    Atmos::Sprite FromAguiSpriteComponent(const agui::SpriteComponent &sprite)
    {
        Atmos::Sprite ret(sprite.GetResource()->GetFileName().c_str(), sprite.GetIndex(), Atmos::Color(sprite.color.alpha, sprite.color.red, sprite.color.green, sprite.color.blue));
        ret.SetXScaler(sprite.GetSize().GetXScaler());
        ret.SetYScaler(sprite.GetSize().GetYScaler());
        return ret;
    }
}