
#include "LightingHandler.h"

#include "LightSource.h"
#include "Canvas.h"
#include "CanvasView.h"
#include "Environment.h"

#include <AGUI/System.h>

// TEST
#include "Debug.h"

namespace Atmos
{
    void LightingHandler::OnResolutionChange(const agui::Resolution &res)
    {
        Environment::GetGraphics()->ResizeCanvas(lightmap, GraphicsHandlerBase::ScreenDimensions(res.GetWidth(), res.GetHeight()));
    }

    LightingHandler::LightingHandler(Field &field) : FieldComponent(field), lightmap(Environment::GetGraphics()->CreateCanvas(Environment::GetGraphics()->GetCurrentDimensions()))
    {
        agui::System::Instance().eventResolutionChanged.Subscribe(std::bind(&LightingHandler::OnResolutionChange, this, std::placeholders::_1));
        lightmapView.SetCanvas(lightmap);
        lightmapView.SetPosition(Position3D(16.0f, 16.0f, 16.0f));
    }

    LightingHandler::LightingHandler(Field &field, LightingHandler &&arg) : FieldComponent(field), lightmap(std::move(arg.lightmap))
    {}

    LightingHandler& LightingHandler::operator=(LightingHandler &&arg)
    {
        lightmap = std::move(lightmap);
        return *this;
    }

    void LightingHandler::Work()
    {
        /*
        lightmap.StartPainting();

        DebugScreen::Instance().Misc1().Start();
        lightmap.Clear(Color(255, 0, 0, 0));
        DebugScreen::Instance().Misc1().Calc();

        for (auto &lightLoop : lights)
            lightLoop->Draw(lightmap);
        lightmap.StopPainting();

        lightmapView.Draw();
        */
    }

    void LightingHandler::Add(LightSource &add)
    {
        lights.emplace(&add);
    }

    void LightingHandler::Remove(LightSource &remove)
    {
        lights.erase(&remove);
    }
}