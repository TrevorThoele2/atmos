#include "Surface.h"

namespace Atmos::Render
{
    void Surface::StageRender(const ImageRender& imageRender) const
    {
        Resource()->StageRender(imageRender);
    }

    void Surface::StageRender(const LineRender& lineRender) const
    {
        Resource()->StageRender(lineRender);
    }

    void Surface::StageRender(const RegionRender& regionRender) const
    {
        Resource()->StageRender(regionRender);
    }

    void Surface::StageRender(const TextRender& textRender) const
    {
        Resource()->StageRender(textRender);
    }

    void Surface::DrawFrame() const
    {
        Resource()->DrawFrame(init.owner, core->backgroundColor);
    }

    Spatial::Size2D Surface::Size() const
    {
        return Resource()->Size();
    }

    auto Surface::Resource() const -> ResourceT*
    {
        return core->resource.get();
    }

    Surface::Surface(Arca::RelicInit init, ResourcePtr&& resource) :
        core(init.Create<Core>(std::move(resource))),
        init(init)
    {
        const auto onDestroying = [this](const auto& signal)
        {
            Resource()->OnMaterialDestroying(signal.index);
        };
        
        init.owner.On<Arca::DestroyingKnown<Asset::Material>>(onDestroying);
    }
}