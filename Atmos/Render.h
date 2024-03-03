
#pragma once

#include "Flags.h"

////////////////////////////
#include "ObjectSystem.h"

namespace Atmos
{
    enum class RenderFlags
    {
        DEFAULT,
        DONT_DRAW
    };

    class RenderHandler
    {
    public:
        Flags<RenderFlags> flags;
    public:
        RenderHandler();
        void Work();
    };

    class RenderingSystem : public ObjectSystem
    {
    public:
        Flags<RenderFlags> flags;
    public:
        RenderingSystem(ObjectManager& manager);
    private:
        void WorkImpl() override;
    };
}