#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "SurfaceCore.h"

#include "GraphicsManager.h"

namespace Atmos::Render
{
    struct MaterialRender;
    struct CanvasRender;
    struct LineRender;

    template<class Derived>
    class Surface : public Arca::ClosedTypedRelic<Derived>
    {
    private:
        using BaseT = Arca::ClosedTypedRelic<Derived>;
    protected:
        using Init = typename BaseT::Init;
    public:
        virtual ~Surface() = 0;

        void RenderStaged();

        [[nodiscard]] ScreenSize Size() const;

        void Reset();
        void Release();
    protected:
        using DataT = SurfaceData;
        using DataPtr = std::unique_ptr<DataT>;
    protected:
        Surface(Init init, DataPtr&& data);

        [[nodiscard]] DataT* Data() const;
        template<class DataT>
        [[nodiscard]] DataT* Data() const;

        [[nodiscard]] Arca::Index<SurfaceCore> Core() const;
    protected:
        virtual void RenderStagedImpl(GraphicsManager& graphicsManager) = 0;
    protected:
        using BaseT::Owner;
    private:
        Arca::Index<SurfaceCore> core;
    };

    template<class Derived>
    Surface<Derived>::~Surface() = default;

    template<class Derived>
    void Surface<Derived>::RenderStaged()
    {
        const auto graphicsManager = Arca::Postulate<GraphicsManager*>(Owner()).Get();
        RenderStagedImpl(*graphicsManager);
    }

    template<class Derived>
    ScreenSize Surface<Derived>::Size() const
    {
        return Data()->Size();
    }

    template<class Derived>
    void Surface<Derived>::Reset()
    {
        Data()->Reset();
    }

    template<class Derived>
    void Surface<Derived>::Release()
    {
        Data()->Release();
    }

    template<class Derived>
    Surface<Derived>::Surface(Init init, DataPtr&& data) :
        Arca::ClosedTypedRelic<Derived>(init),
        core(init.template Create<SurfaceCore>(std::move(data)))
    {}

    template<class Derived>
    auto Surface<Derived>::Data() const -> DataT*
    {
        return core->data.get();
    }

    template<class Derived>
    template<class DataT>
    DataT* Surface<Derived>::Data() const
    {
        return static_cast<DataT*>(core->data.get());
    }

    template<class Derived>
    Arca::Index<SurfaceCore> Surface<Derived>::Core() const
    {
        return core;
    }
}