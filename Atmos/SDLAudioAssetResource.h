#pragma once

#include "AudioAssetResource.h"

#include "SDLIncludes.h"

#include "Buffer.h"

namespace Atmos::Asset::Resource::SDL
{
    class Audio final : public Resource::Audio
    {
    public:
        Buffer buffer;

        SDL_RWops* ops = nullptr;
    public:
        explicit Audio(const Buffer& buffer);
        ~Audio();
    };
}