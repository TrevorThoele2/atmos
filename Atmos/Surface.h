#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "SurfaceCore.h"

namespace Atmos::Render
{
    struct ImageRender;
    struct LineRender;
    struct RegionRender;

    template<class Derived>
    class Surface : public Arca::ClosedTypedRelic<Derived>
    {
    private:
        using BaseT = Arca::ClosedTypedRelic<Derived>;
    protected:
        using Init = typename BaseT::Init;
    public:
        using ResourceT = Resource::Surface;
    public:
        virtual ~Surface() = 0;

        void StageRender(const ImageRender& imageRender) const;
        void StageRender(const LineRender& lineRender) const;
        void StageRender(const RegionRender& regionRender) const;

        void DrawFrame() const;

        [[nodiscard]] ScreenSize Size() const;

        [[nodiscard]] ResourceT* Resource() const;
        template<class ResourceT>
        [[nodiscard]] ResourceT* Resource() const;
    protected:
        using ResourcePtr = std::unique_ptr<ResourceT>;
    protected:
        Surface(Init init, ResourcePtr&& resource);

        [[nodiscard]] Arca::Index<SurfaceCore> Core() const;
    protected:
        using BaseT::Owner;
    private:
        Arca::Index<SurfaceCore> core;
    };

    template<class Derived>
    Surface<Derived>::~Surface() = default;

    template<class Derived>
    void Surface<Derived>::StageRender(const ImageRender& imageRender) const
    {
        Resource()->StageRender(imageRender);
    }

    template<class Derived>
    void Surface<Derived>::StageRender(const LineRender& lineRender) const
    {
        Resource()->StageRender(lineRender);
    }

    template<class Derived>
    void Surface<Derived>::StageRender(const RegionRender& regionRender) const
    {
        Resource()->StageRender(regionRender);
    }

    template<class Derived>
    void Surface<Derived>::DrawFrame() const
    {
        Resource()->DrawFrame(Owner(), Core()->backgroundColor);
    }

    template<class Derived>
    ScreenSize Surface<Derived>::Size() const
    {
        return Resource()->Size();
    }

    template<class Derived>
    auto Surface<Derived>::Resource() const -> ResourceT*
    {
        return core->resource.get();
    }

    template<class Derived>
    template<class ResourceT>
    ResourceT* Surface<Derived>::Resource() const
    {
        return static_cast<ResourceT*>(core->resource.get());
    }

    template<class Derived>
    Surface<Derived>::Surface(Init init, ResourcePtr&& resource) :
        Arca::ClosedTypedRelic<Derived>(init),
        core(init.template Create<SurfaceCore>(std::move(resource)))
    {}

    template<class Derived>
    Arca::Index<SurfaceCore> Surface<Derived>::Core() const
    {
        return core;
    }
}