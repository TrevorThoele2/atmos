#pragma once

#include "ArcaRelicIncludes.h"
#include "SurfaceCore.h"

namespace Atmos::Render
{
    struct RenderImage;
    struct RenderLine;
    struct RenderRegion;
    struct RenderText;

    class Surface
    {
    public:
        using ResourceT = SurfaceCore::ResourceT;
        using ResourcePtr = SurfaceCore::ResourcePtr;

        using Core = SurfaceCore;
        Arca::Index<Core> core;
    public:
        Surface(const Surface& arg) = delete;
        Surface(Surface&& arg) noexcept = default;

        Surface& operator=(const Surface& arg) = delete;
        Surface& operator=(Surface&& arg) = default;
        
        [[nodiscard]] Spatial::Size2D Size() const;

        [[nodiscard]] ResourceT* Resource() const;
        template<class ResourceT>
        [[nodiscard]] ResourceT* Resource() const;
    protected:
        Surface(Arca::RelicInit init, ResourcePtr&& resource);
    private:
        Arca::RelicInit init;
    };

    template<class ResourceT>
    ResourceT* Surface::Resource() const
    {
        return static_cast<ResourceT*>(core->resource.get());
    }
}