#include "DirectX9ImageAssetResource.h"

namespace Atmos::Asset::Resource::DirectX9
{
    Image::Image(LPDIRECT3DTEXTURE9 texture) :
        texture(texture)
    {}

    Image::~Image()
    {
        texture->Release();
    }

    LPDIRECT3DTEXTURE9 Image::Texture() const
    {
        return texture;
    }
}