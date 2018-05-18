
#pragma once

#include "Color.h"
#include "Flags.h"

namespace Atmos
{
    enum class RenderFlags
    {
        DEFAULT,
        DONT_DRAW
    };

    class RenderHandler
    {
    private:
        RenderHandler();
        RenderHandler(const RenderHandler &arg) = delete;
        RenderHandler& operator=(const RenderHandler &arg) = delete;
    public:
        Flags<RenderFlags> flags;
        static RenderHandler& Instance();
        static void Render();
    };
}