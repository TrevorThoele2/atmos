#include "SDLFontAssetResource.h"

#include "GraphicsError.h"
#include "SDLText.h"

namespace Atmos::Render::SDL
{
    FontAssetResource::FontAssetResource(const String& name, const Buffer& buffer) :
        name(name), buffer(buffer)
    {
        ops = SDL_RWFromMem(this->buffer.data(), this->buffer.size());
        if (!ops)
            throw GraphicsError("Could not create font asset resource.", { {"Reason", String(SDL_GetError())} });
        
        font = TTF_OpenFontRW(ops, 0, 18);
        if (!font)
            throw GraphicsError(
                "Could not open TTF font.",
                {
                    { "Name", name },
                    { "Reason", String(TTF_GetError()) }
                });
    }

    FontAssetResource::~FontAssetResource()
    {
        if (font)
            TTF_CloseFont(font);
    }

    Spatial::Size2D FontAssetResource::Size(const String& string, bool bold, bool italics) const
    {
        SetStyle(*font, Style(bold, italics));
        return SDL::Size(*font, Split(string));
    }
}