#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "SurfaceCore.h"

namespace Atmos::Render
{
    struct ImageRender;
    struct LineRender;
    struct RegionRender;

    template<class DerivedT>
    class Surface : public Arca::ClosedTypedRelic<DerivedT>
    {
    private:
        using BaseT = Arca::ClosedTypedRelic<DerivedT>;
    public:
        using Init = typename BaseT::Init;
    protected:
        using BaseT::Owner;
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

        void StageRender(const ImageRender& imageRender) const;
        void StageRender(const LineRender& lineRender) const;
        void StageRender(const RegionRender& regionRender) const;

        void DrawFrame() const;

        [[nodiscard]] Spatial::ScreenSize Size() const;

        [[nodiscard]] ResourceT* Resource() const;
        template<class ResourceT>
        [[nodiscard]] ResourceT* Resource() const;
    protected:
        Surface(Init init, ResourcePtr&& resource);
    };

    template<class DerivedT>
    void Surface<DerivedT>::StageRender(const ImageRender& imageRender) const
    {
        Resource()->StageRender(imageRender);
    }

    template<class DerivedT>
    void Surface<DerivedT>::StageRender(const LineRender& lineRender) const
    {
        Resource()->StageRender(lineRender);
    }

    template<class DerivedT>
    void Surface<DerivedT>::StageRender(const RegionRender& regionRender) const
    {
        Resource()->StageRender(regionRender);
    }

    template<class DerivedT>
    void Surface<DerivedT>::DrawFrame() const
    {
        Resource()->DrawFrame(Owner(), core->backgroundColor);
    }

    template<class DerivedT>
    Spatial::ScreenSize Surface<DerivedT>::Size() const
    {
        return Resource()->Size();
    }

    template<class DerivedT>
    auto Surface<DerivedT>::Resource() const -> ResourceT*
    {
        return core->resource.get();
    }

    template<class DerivedT>
    template<class ResourceT>
    ResourceT* Surface<DerivedT>::Resource() const
    {
        return static_cast<ResourceT*>(core->resource.get());
    }

    template<class DerivedT>
    Surface<DerivedT>::Surface(Init init, ResourcePtr&& resource) :
        Arca::ClosedTypedRelic<DerivedT>(init),
        core(init.template Create<Core>(std::move(resource)))
    {
        Owner().template On<Arca::CreatedKnown<Asset::Material>>(
            [this](const Arca::CreatedKnown<Asset::Material>& signal)
            {
                Resource()->OnMaterialCreated(signal.index);
            });
        Owner().template On<Arca::DestroyingKnown<Asset::Material>>(
            [this](const Arca::DestroyingKnown<Asset::Material>& signal)
            {
                Resource()->OnMaterialDestroying(signal.index);
            });
    }
}