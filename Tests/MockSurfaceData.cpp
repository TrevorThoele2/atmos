#include "MockSurfaceData.h"

void MockSurfaceDataImplementation::StageRender(const Atmos::Render::ImageRender& imageRender)
{
    imageRenders.push_back(imageRender);
}

void MockSurfaceDataImplementation::StageRender(const Atmos::Render::CanvasRender& canvasRender)
{
    canvasRenders.push_back(canvasRender);
}

void MockSurfaceDataImplementation::StageRender(const Atmos::Render::LineRender& lineRender)
{
    lineRenders.push_back(lineRender);
}

void MockSurfaceDataImplementation::DrawFrame(const Atmos::Render::Color& backgroundColor)
{
    
}

Atmos::ScreenSize MockSurfaceDataImplementation::Size() const
{
    return {};
}