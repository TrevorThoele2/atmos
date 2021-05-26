#pragma once

#include "FontAssetResource.h"
#include <SDL_ttf.h>
#include "Buffer.h"

namespace Atmos::Render::SDL
{
    class FontAssetResource final : public Asset::Resource::Font
    {
    public:
        String name;

        TTF_Font* font;

        Buffer buffer;
        SDL_RWops* ops;
    public:
        FontAssetResource(const String& name, const Buffer& buffer);
        ~FontAssetResource();
    };
}