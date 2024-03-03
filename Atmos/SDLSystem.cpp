#include "SDLSystem.h"

#include <SDL.h>
#include "SystemCreationFailed.h"

namespace Atmos::System
{
    SDL::SDL()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
            throw SystemCreationFailed(String(SDL_GetError()));
    }

    SDL::~SDL()
    {
        SDL_Quit();
    }
}