#pragma once

#include "ImageAssetData.h"
#include "DirectX9Includes.h"

namespace Atmos::Render::DirectX9
{
    class ImageAssetDataImplementation final : public Asset::ImageAssetData
    {
    public:
        explicit ImageAssetDataImplementation(LPDIRECT3DTEXTURE9 texture, Dimension width, Dimension height);
        ~ImageAssetDataImplementation();

        [[nodiscard]] LPDIRECT3DTEXTURE9 Texture() const;
    private:
        LPDIRECT3DTEXTURE9 texture;
    };
}