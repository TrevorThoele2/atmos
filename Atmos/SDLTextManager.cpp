#include "SDLTextManager.h"

#include "SDLTextError.h"
#include "SDLText.h"
#include <utf8.h>
#include "GraphicsError.h"

namespace Atmos::Render::SDL
{
    TextManager::TextManager()
    {
        VerifyResult(
            TTF_Init(),
            "Could not initialize SDL text manager.");
    }

	std::unique_ptr<Asset::Resource::Font> TextManager::CreateFontResource(const Buffer& buffer, const Name& name) const
    {
		return std::make_unique<FontAssetResource>(name, buffer);
    }
}