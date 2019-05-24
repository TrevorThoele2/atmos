#pragma once

#include "ObjectSystem.h"

#include "Flags.h"

namespace Atmos
{
    class GraphicsSystem;
    class RenderFragmentSystem;
    class DebugStatisticsSystem;

    enum class RenderFlags
    {
        DEFAULT,
        DONT_DRAW
    };

    class RenderingSystem : public ObjectSystem
    {
    public:
        Flags<RenderFlags> flags;
    public:
        RenderingSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(RenderingSystem);
    private:
        void WorkImpl() override;
    private:
        GraphicsSystem* FindGraphicsSystem();
        RenderFragmentSystem* FindRenderFragmentSystem();
        DebugStatisticsSystem* FindDebugStatisticsSystem();
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::RenderingSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}