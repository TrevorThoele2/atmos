#include "MockSurfaceResource.h"

void MockSurfaceResourceImplementation::StageRender(const Atmos::Render::ImageRender& imageRender)
{
    imageRenders.push_back(imageRender);
}

void MockSurfaceResourceImplementation::StageRender(const Atmos::Render::LineRender& lineRender)
{
    lineRenders.push_back(lineRender);
}

void MockSurfaceResourceImplementation::StageRender(const Atmos::Render::RegionRender& regionRender)
{
    regionRenders.push_back(regionRender);
}

void MockSurfaceResourceImplementation::DrawFrame(Arca::Reliquary& reliquary, const Atmos::Render::Color& backgroundColor)
{
    
}

Atmos::Spatial::ScreenSize MockSurfaceResourceImplementation::Size() const
{
    return {};
}