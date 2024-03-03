#pragma once

#include "ImageAssetData.h"
#include "DirectX9Includes.h"

namespace Atmos::Render::DirectX9
{
    class ImageAssetDataImplementation final : public Asset::ImageData
    {
    public:
        explicit ImageAssetDataImplementation(LPDIRECT3DTEXTURE9 texture);
        ~ImageAssetDataImplementation();

        [[nodiscard]] LPDIRECT3DTEXTURE9 Texture() const;
    private:
        LPDIRECT3DTEXTURE9 texture;
    };
}