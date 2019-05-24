
#include "RenderingSystem.h"

#include "ObjectManager.h"
#include "RenderFragmentSystem.h"
#include "GraphicsSystem.h"
#include "DebugStatisticsSystem.h"

#include "GraphicsManager.h"

namespace Atmos
{
    RenderingSystem::RenderingSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(RenderingSystem) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
    {}

    void RenderingSystem::WorkImpl()
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

        if (!flags.Get(RenderFlags::DONT_DRAW))
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

    GraphicsSystem* RenderingSystem::FindGraphicsSystem()
    {
        return Manager()->FindSystem<GraphicsSystem>();
    }

    RenderFragmentSystem* RenderingSystem::FindRenderFragmentSystem()
    {
        return Manager()->FindSystem<RenderFragmentSystem>();
    }

    DebugStatisticsSystem* RenderingSystem::FindDebugStatisticsSystem()
    {
        return Manager()->FindSystem<DebugStatisticsSystem>();
    }
}

namespace Inscription
{
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::RenderingSystem)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::RenderingSystem, "RenderingSystem");
}