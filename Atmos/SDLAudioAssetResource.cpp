#include "SDLAudioAssetResource.h"

#include "AudioError.h"

namespace Atmos::Asset::Resource::SDL
{
    Audio::Audio(const Buffer& buffer) : buffer(buffer)
    {
        ops = SDL_RWFromMem(this->buffer.data(), this->buffer.size());
        if (!ops)
            throw Atmos::Audio::Error("Could not create audio asset resource.", { {"Reason", String(SDL_GetError())} });
    }

    Audio::~Audio()
    {
        if (ops)
            SDL_FreeRW(ops);
    }
}