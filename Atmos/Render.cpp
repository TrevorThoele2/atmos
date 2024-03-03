
#include "Render.h"

#include "Environment.h"
#include "GameEnvironment.h"

#include "WorldManager.h"
#include "Debug.h"

#include <AGUI\System.h>

namespace Atmos
{
    RenderHandler::RenderHandler() : flags(RenderFlags::DEFAULT)
    {}

    void RenderHandler::Work()
    {
        DebugScreen::Render().Start();

        auto graphics = Environment::GetGraphics();
        if (!graphics->Start())
        {
            DebugScreen::Render().Calc();
            return;
        }

        graphics->StartSprites();
        // Clear main and stencil
        {
            typedef GraphicsHandlerBase::Target Target;
            DebugScreen::Instance().Misc2().Start();
            graphics->ClearTarget(Flags<Target>({ Target::MAIN }), Color(0, 255, 0, 0));
            DebugScreen::Instance().Misc2().Calc();
        }

        if (!flags.Get(RenderFlags::DONT_DRAW))
            GameEnvironment::GetWorldManager().Draw();

        graphics->Flush();

        graphics->EndSprites();

        Agui::System::Work();
        graphics->End();
        graphics->Present();

        DebugScreen::Render().Calc();
    }

    RenderingSystem::RenderingSystem(ObjectManager& manager) : ObjectSystem(manager), flags(RenderFlags::DEFAULT)
    {}

    void RenderingSystem::WorkImpl()
    {
        DebugScreen::Render().Start();

        auto graphics = Environment::GetGraphics();
        if (!graphics->Start())
        {
            DebugScreen::Render().Calc();
            return;
        }

        graphics->StartSprites();
        // Clear main and stencil
        {
            typedef GraphicsHandlerBase::Target Target;
            DebugScreen::Instance().Misc2().Start();
            graphics->ClearTarget(Flags<Target>({ Target::MAIN }), Color(0, 255, 0, 0));
            DebugScreen::Instance().Misc2().Calc();
        }

        if (!flags.Get(RenderFlags::DONT_DRAW))
            GameEnvironment::GetWorldManager().Draw();

        graphics->Flush();

        graphics->EndSprites();

        Agui::System::Work();
        graphics->End();
        graphics->Present();

        DebugScreen::Render().Calc();
    }
}