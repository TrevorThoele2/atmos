#pragma once

#include "ObjectSystem.h"

#include "Flags.h"

namespace Atmos
{
    class GraphicsSystem;
    class FragmentSystem;
    class DebugStatisticsSystem;
}

namespace Atmos::Render
{
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
    private:
        void WorkImpl() override;
    private:
        GraphicsSystem* FindGraphicsSystem();
        FragmentSystem* FindRenderFragmentSystem();
        DebugStatisticsSystem* FindDebugStatisticsSystem();
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::RenderSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Render::RenderSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}