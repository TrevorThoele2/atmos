#pragma once

#include "TextManager.h"
#include "SDLFontAssetResource.h"

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