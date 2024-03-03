
#include "Render.h"

#include "Environment.h"

#include "WorldManager.h"
#include "Debug.h"

#include <AGUI\System.h>

namespace Atmos
{
    RenderHandler::RenderHandler() : flags(RenderFlags::DEFAULT)
    {}

    RenderHandler& RenderHandler::Instance()
    {
        static RenderHandler instance;
        return instance;
    }

    void RenderHandler::Render()
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

        if (!Instance().flags.Get(RenderFlags::DONT_DRAW))
            WorldManager::Draw();

        graphics->Flush();

        graphics->EndSprites();

        agui::System::Work();
        graphics->End();
        graphics->Present();

        DebugScreen::Render().Calc();
    }
}