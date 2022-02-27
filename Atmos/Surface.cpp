#include "Surface.h"

namespace Atmos::Render
{
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
        init.owner.On<Arca::DestroyingKnown<Asset::Material>>([this](const auto& signal)
            {
                Resource()->OnMaterialDestroying(*signal.index);
            });
    }
}