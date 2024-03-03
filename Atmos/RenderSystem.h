#pragma once

#include "ObjectSystem.h"

#include "Flags.h"

namespace Atmos
{
    class DebugStatisticsSystem;
}

namespace Atmos::Render
{
    class GraphicsSystem;
    class RenderFragmentSystem;

    class RenderSystem : public ObjectSystem
    {
    public:
        enum class Flag
        {
            DEFAULT,
            DONT_DRAW
        };

        Flags<Flag> flags;
    public:
        RenderSystem(ObjectManager& manager);
    protected:
        void WorkImpl() override;
    private:
        GraphicsSystem* FindGraphicsSystem();
        RenderFragmentSystem* FindRenderFragmentSystem();
        DebugStatisticsSystem* FindDebugStatisticsSystem();
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::RenderSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Render::RenderSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}