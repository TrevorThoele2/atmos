#pragma once

#include <unordered_set>

#include "FieldComponent.h"
#include "CanvasView.h"

#include <AGUI/Resolution.h>

namespace Atmos
{
    class Canvas;
    class LightSource;
    class ShaderAsset;
    class LightingHandler : public FieldComponent
    {
    private:
        std::unordered_set<LightSource*> lights;

        Canvas &lightmap;
        CanvasView lightmapView;

        LightingHandler(const LightingHandler &arg) = delete;
        LightingHandler& operator=(const LightingHandler &arg) = delete;

        void OnResolutionChange(const agui::Resolution &res);
    public:
        LightingHandler(Field &field);
        LightingHandler(Field &field, LightingHandler &&arg);
        LightingHandler& operator=(LightingHandler &&arg);

        void Work();

        void Add(LightSource &add);
        void Remove(LightSource &remove);
    };
}