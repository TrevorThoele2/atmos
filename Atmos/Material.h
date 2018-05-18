#pragma once

#include "Asset.h"

#include "ImageAsset.h"
#include "Shader.h"
#include "AssetReference.h"
#include "Name.h"

#include "Optional.h"

#include "Serialization.h"

namespace Atmos
{
    class Material : public Asset
    {
    public:
        typedef ImageAsset::Dimension Dimension;
        typedef ImageAsset::GridDimension GridDimension;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        struct Side
        {
            AssetReference<ImageAsset> visual;
            AssetReference<ImageAsset> normal;
            AssetReference<ImageAsset> height;
            Side();
            bool operator==(const Side &arg) const;
            bool operator!=(const Side &arg) const;
            bool IsEmpty() const;
        };
    private:
        static const Dimension invalidDim = 0;
        static const GridDimension invalidGridDim = 0;

        Name name;
        Dimension width, height;
        GridDimension columns, rows;

        // Side you always see (front/back)
        Side xSide;
        // Left/right sides
        Optional<Side> ySide;
        // Top/bottom sides
        Optional<Side> zSide;

        AssetReference<ShaderAsset> shader;

        // If yes, then this material participates in lighting
        // Otherwise, normal and height and other sides don't affect anything at all
        // Additional, it will not block light/shadows
        bool lightingEnabled;

        // Returns true if the image was set
        bool SetImageCommon(Side *side, AssetReference<ImageAsset>(Side::*image), AssetReference<ImageAsset> set);

        String GetStringImpl() const override final;
    public:
        Material(const Name &name);
        Material(const Material &arg) = default;
        Material& operator=(const Material &arg) = default;

        bool operator==(const Material &arg) const;
        bool operator!=(const Material &arg) const;

        const Name& GetName() const;

        // All of the image assets used by this material should be the same dimensions (including the grid for it)
        void SetXVisual(AssetReference<ImageAsset> set);
        // All of the image assets used by this material should be the same dimensions (including the grid for it)
        void SetXNormal(AssetReference<ImageAsset> set);
        // All of the image assets used by this material should be the same dimensions (including the grid for it)
        void SetXHeight(AssetReference<ImageAsset> set);
        // All of the image assets used by this material should be the same dimensions (including the grid for it)
        void SetYVisual(AssetReference<ImageAsset> set);
        // All of the image assets used by this material should be the same dimensions (including the grid for it)
        void SetYNormal(AssetReference<ImageAsset> set);
        // All of the image assets used by this material should be the same dimensions (including the grid for it)
        void SetYHeight(AssetReference<ImageAsset> set);
        // All of the image assets used by this material should be the same dimensions (including the grid for it)
        void SetZVisual(AssetReference<ImageAsset> set);
        // All of the image assets used by this material should be the same dimensions (including the grid for it)
        void SetZNormal(AssetReference<ImageAsset> set);
        // All of the image assets used by this material should be the same dimensions (including the grid for it)
        void SetZHeight(AssetReference<ImageAsset> set);
        AssetReference<ImageAsset> GetXVisual() const;
        AssetReference<ImageAsset> GetXNormal() const;
        AssetReference<ImageAsset> GetXHeight() const;
        AssetReference<ImageAsset> GetYVisual() const;
        AssetReference<ImageAsset> GetYNormal() const;
        AssetReference<ImageAsset> GetYHeight() const;
        AssetReference<ImageAsset> GetZVisual() const;
        AssetReference<ImageAsset> GetZNormal() const;
        AssetReference<ImageAsset> GetZHeight() const;

        void SetShader(AssetReference<ShaderAsset> set);
        AssetReference<ShaderAsset> GetShader() const;

        // Assumes that all of the image assets used are the same
        Dimension GetWidth() const;
        // Assumes that all of the image assets used are the same
        Dimension GetHeight() const;

        // Assumes that all of the image assets used are the same
        GridDimension GetColumns() const;
        // Assumes that all of the image assets used are the same
        GridDimension GetRows() const;

        void SetLightingEnabled(bool set = true);
        bool IsLightingEnabled() const;
    };
}