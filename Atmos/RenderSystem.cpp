#include "RenderSystem.h"

#include "ObjectManager.h"
#include "RenderFragmentSystem.h"
#include "GraphicsSystem.h"
#include "DebugStatisticsSystem.h"

#include "GraphicsManager.h"

namespace Atmos::Render
{
    RenderSystem::RenderSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    void RenderSystem::WorkImpl()
    {
        auto graphics = FindGraphicsSystem()->Get();
        auto debugStatistics = FindDebugStatisticsSystem();

        debugStatistics->renderProfiler.Start();

        if (!graphics->Start())
        {
            debugStatistics->renderProfiler.Calculate();
            return;
        }

        graphics->StartSprites();
        // Clear main and stencil
        {
            typedef GraphicsManager::Target Target;
            graphics->ClearTarget(Flags<Target>({ Target::MAIN }), Color(0, 255, 0, 0));
        }

        if (!flags.Get(Flag::DONT_DRAW))
        {
            auto renderFragmentSystem = FindRenderFragmentSystem();
            renderFragmentSystem->DrawAll();
        }

        graphics->Flush();

        graphics->EndSprites();

        graphics->End();
        graphics->Present();

        debugStatistics->renderProfiler.Calculate();
    }

    GraphicsSystem* RenderSystem::FindGraphicsSystem()
    {
        return Manager()->FindSystem<GraphicsSystem>();
    }

    RenderFragmentSystem* RenderSystem::FindRenderFragmentSystem()
    {
        return Manager()->FindSystem<RenderFragmentSystem>();
    }

    DebugStatisticsSystem* RenderSystem::FindDebugStatisticsSystem()
    {
        return Manager()->FindSystem<DebugStatisticsSystem>();
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Render::RenderSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}