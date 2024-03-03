#include "DirectX9ImageAssetData.h"

namespace Atmos::Render::DirectX9
{
    ImageAssetDataImplementation::ImageAssetDataImplementation(LPDIRECT3DTEXTURE9 texture) : texture(texture)
    {}

    ImageAssetDataImplementation::~ImageAssetDataImplementation()
    {
        texture->Release();
    }

    std::unique_ptr<Asset::ImageAssetData> ImageAssetDataImplementation::Clone() const
    {
        return std::make_unique<ImageAssetDataImplementation>(*this);
    }

    LPDIRECT3DTEXTURE9 ImageAssetDataImplementation::Texture() const
    {
        return texture;
    }
}