#pragma once

#include "ImageAssetResource.h"
#include "DirectX9Includes.h"

namespace Atmos::Asset::Resource::DirectX9
{
    class Image final : public Resource::Image
    {
    public:
        explicit Image(LPDIRECT3DTEXTURE9 texture);
        ~Image();

        [[nodiscard]] LPDIRECT3DTEXTURE9 Texture() const;
    private:
        LPDIRECT3DTEXTURE9 texture;
    };
}