
#include "Material.h"

#include "ImageRegistry.h"
#include "ShaderRegistry.h"

#include "BitConcat.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Material)
    {
        // Concatenate the lighting enabled bool + side validity
        BitConcatenator<unsigned char> validityConcat;
        if (scribe.IsOutput())
        {
            validityConcat.PushBitBack(shader != nullptr);
            validityConcat.PushBitBack(zSide.IsValid());
            validityConcat.PushBitBack(ySide.IsValid());
            validityConcat.PushBitBack(lightingEnabled);
        }

        scribe(validityConcat);

        scribe(xSide.visual);
        if (validityConcat[0]) // LightingEnabled
        {
            scribe(xSide.normal);
            scribe(xSide.height);

            if (validityConcat[1]) // ySide valid
            {
                scribe(ySide->visual);
                scribe(ySide->normal);
                scribe(ySide->height);
            }

            if (validityConcat[2]) // zSide valid
            {
                scribe(zSide->visual);
                scribe(zSide->normal);
                scribe(zSide->height);
            }
        }

        if (validityConcat[3]) // Shader valid
            scribe(shader);

        // Reconstruct width/height and columns/rows
        if (scribe.IsInput())
        {
            auto assigner = [&](const Side *side, AssetReference<ImageAsset>(Side::*var))
            {
                if (side && (side->*var))
                {
                    width = (side->*var)->GetWidth();
                    height = (side->*var)->GetHeight();
                    columns = (side->*var)->GetColumns();
                    rows = (side->*var)->GetRows();
                    return true;
                }

                return false;
            };

            if (assigner(&xSide, &Side::visual))
                return;

            if (assigner(&xSide, &Side::normal))
                return;

            if (assigner(&xSide, &Side::height))
                return;

            if (assigner(ySide.GetAsPtr(), &Side::visual))
                return;

            if (assigner(ySide.GetAsPtr(), &Side::normal))
                return;

            if (assigner(ySide.GetAsPtr(), &Side::height))
                return;

            if (assigner(zSide.GetAsPtr(), &Side::visual))
                return;

            if (assigner(zSide.GetAsPtr(), &Side::normal))
                return;

            if (assigner(zSide.GetAsPtr(), &Side::height))
                return;
        }
    }

    Material::Side::Side() : visual(nullptr), normal(nullptr), height(nullptr)
    {}

    bool Material::Side::operator==(const Side &arg) const
    {
        return visual == arg.visual && normal == arg.normal && height == arg.height;
    }

    bool Material::Side::operator!=(const Side &arg) const
    {
        return !(*this == arg);
    }

    bool Material::Side::IsEmpty() const
    {
        return !visual && !normal && !height;
    }

    bool Material::SetImageCommon(Side *side, AssetReference<ImageAsset>(Side::*image), AssetReference<ImageAsset> set)
    {
        if (!side)
            return true;

        if (set)
        {
            // Check if this material has no images
            if (width == invalidDim)
            {
                // Setup the material
                (side->*image) = set;
                width = set->GetWidth();
                height = set->GetHeight();
                columns = set->GetColumns();
                rows = set->GetRows();
                return true;
            }
            // Check if the image can be set via dimensions
            else if (width != set->GetWidth() || height != set->GetHeight() || columns != set->GetColumns() || rows != set->GetRows())
                return false;

            // Set normally
            (side->*image) = set;
            return true;
        }

        // Reset the image
        (side->*image) = set;
        // Check if everything is reset, if so need to reset the dimensions
        if (xSide.IsEmpty() && (!ySide || ySide->IsEmpty()) && (!zSide || zSide->IsEmpty()))
        {
            width = invalidDim;
            height = invalidDim;
            columns = invalidGridDim;
            rows = invalidGridDim;
        }

        return true;
    }

    String Material::GetStringImpl() const
    {
        return name;
    }

    Material::Material(const Name &name) : name(name), width(invalidDim), height(invalidDim), columns(invalidDim), rows(invalidDim), lightingEnabled(true)
    {}

    bool Material::operator==(const Material &arg) const
    {
        return Asset::operator==(arg);
    }

    bool Material::operator!=(const Material &arg) const
    {
        return !(*this == arg);
    }

    const Name& Material::GetName() const
    {
        return name;
    }

    void Material::SetXVisual(AssetReference<ImageAsset> set)
    {
        SetImageCommon(&xSide, &Side::visual, set);
    }

    void Material::SetXNormal(AssetReference<ImageAsset> set)
    {
        SetImageCommon(&xSide, &Side::normal, set);
    }

    void Material::SetXHeight(AssetReference<ImageAsset> set)
    {
        SetImageCommon(&xSide, &Side::height, set);
    }

    void Material::SetYVisual(AssetReference<ImageAsset> set)
    {
        if (set && !ySide.IsValid())
            ySide.Set(Side());

        SetImageCommon(ySide.GetAsPtr(), &Side::visual, set);
    }

    void Material::SetYNormal(AssetReference<ImageAsset> set)
    {
        if (set && !ySide.IsValid())
            ySide.Set(Side());

        SetImageCommon(ySide.GetAsPtr(), &Side::normal, set);
    }

    void Material::SetYHeight(AssetReference<ImageAsset> set)
    {
        if (set && !ySide.IsValid())
            ySide.Set(Side());

        SetImageCommon(ySide.GetAsPtr(), &Side::height, set);
    }

    void Material::SetZVisual(AssetReference<ImageAsset> set)
    {
        if (set && !zSide.IsValid())
            zSide.Set(Side());

        SetImageCommon(zSide.GetAsPtr(), &Side::visual, set);
    }

    void Material::SetZNormal(AssetReference<ImageAsset> set)
    {
        if (set && !zSide.IsValid())
            zSide.Set(Side());

        SetImageCommon(zSide.GetAsPtr(), &Side::normal, set);
    }

    void Material::SetZHeight(AssetReference<ImageAsset> set)
    {
        if (set && !zSide.IsValid())
            zSide.Set(Side());

        SetImageCommon(zSide.GetAsPtr(), &Side::height, set);
    }

    AssetReference<ImageAsset> Material::GetXVisual() const
    {
        return xSide.visual;
    }

    AssetReference<ImageAsset> Material::GetXNormal() const
    {
        return xSide.normal;
    }

    AssetReference<ImageAsset> Material::GetXHeight() const
    {
        return xSide.height;
    }

    AssetReference<ImageAsset> Material::GetYVisual() const
    {
        if (!ySide)
            return nullptr;

        return ySide->visual;
    }

    AssetReference<ImageAsset> Material::GetYNormal() const
    {
        if (!ySide)
            return nullptr;

        return ySide->normal;
    }

    AssetReference<ImageAsset> Material::GetYHeight() const
    {
        if (!ySide)
            return nullptr;

        return ySide->height;
    }

    AssetReference<ImageAsset> Material::GetZVisual() const
    {
        if (!zSide)
            return nullptr;

        return zSide->visual;
    }

    AssetReference<ImageAsset> Material::GetZNormal() const
    {
        if (!zSide)
            return nullptr;

        return zSide->normal;
    }

    AssetReference<ImageAsset> Material::GetZHeight() const
    {
        if (!zSide)
            return nullptr;

        return zSide->height;
    }

    void Material::SetShader(AssetReference<ShaderAsset> set)
    {
        shader = set;
    }

    AssetReference<ShaderAsset> Material::GetShader() const
    {
        return shader;
    }

    Material::Dimension Material::GetWidth() const
    {
        return width;
    }

    Material::Dimension Material::GetHeight() const
    {
        return height;
    }

    Material::GridDimension Material::GetColumns() const
    {
        return columns;
    }

    Material::GridDimension Material::GetRows() const
    {
        return rows;
    }

    void Material::SetLightingEnabled(bool set)
    {
        lightingEnabled = set;
    }

    bool Material::IsLightingEnabled() const
    {
        return lightingEnabled;
    }
}