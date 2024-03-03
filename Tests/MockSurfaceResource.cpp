#include "MockSurfaceResource.h"

void MockSurfaceResource::StageRender(const Atmos::Render::ImageRender& imageRender)
{
    imageRenders.push_back(imageRender);
}

void MockSurfaceResource::StageRender(const Atmos::Render::LineRender& lineRender)
{
    lineRenders.push_back(lineRender);
}

void MockSurfaceResource::StageRender(const Atmos::Render::RegionRender& regionRender)
{
    regionRenders.push_back(regionRender);
}

void MockSurfaceResource::StageRender(const Atmos::Render::TextRender& textRender)
{
    textRenders.push_back(textRender);
}

void MockSurfaceResource::DrawFrame(Arca::Reliquary& reliquary, const Atmos::Render::Color& backgroundColor)
{
    
}

void MockSurfaceResource::OnMaterialDestroying(const Arca::Index<Atmos::Asset::Material>& material)
{
    
}

Atmos::Spatial::Size2D MockSurfaceResource::Size() const
{
    return {};
}