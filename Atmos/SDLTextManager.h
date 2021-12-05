#pragma once

#include "TextManager.h"
#include <SDL_ttf.h>
#include "SDLFontAssetResource.h"
#include "Color.h"

namespace Atmos::Render::SDL
{
    class TextManager final : public Render::TextManager
    {
    public:
        TextManager();

        [[nodiscard]] std::unique_ptr<Asset::Resource::Font> CreateFontResource(
            const Buffer& buffer, const Name& name) const override;
    };
}