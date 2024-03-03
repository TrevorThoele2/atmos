#pragma once

#include "ImageAssetData.h"
#include "DirectX9Includes.h"

namespace Atmos::Render::DirectX9
{
    class ImageAssetDataImplementation final : public Asset::ImageAssetData
    {
    public:
        explicit ImageAssetDataImplementation(LPDIRECT3DTEXTURE9 texture);
        ~ImageAssetDataImplementation();

        [[nodiscard]] std::unique_ptr<ImageAssetData> Clone() const override;

        [[nodiscard]] LPDIRECT3DTEXTURE9 Texture() const;
    private:
        LPDIRECT3DTEXTURE9 texture;
    };
}