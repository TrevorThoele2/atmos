#include "DirectX9ImageAssetData.h"

namespace Atmos::Render::DirectX9
{
    ImageAssetDataImplementation::ImageAssetDataImplementation(LPDIRECT3DTEXTURE9 texture) :
        texture(texture)
    {}

    ImageAssetDataImplementation::~ImageAssetDataImplementation()
    {
        texture->Release();
    }

    LPDIRECT3DTEXTURE9 ImageAssetDataImplementation::Texture() const
    {
        return texture;
    }
}