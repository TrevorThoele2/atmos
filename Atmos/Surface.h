#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "SurfaceCore.h"

#include "GraphicsManager.h"

#include "Color.h"

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
        void RenderStaged();

        void FullColor(const Color& color = Color());

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
    protected:
        using BaseT::Owner;
    private:
        using Core = SurfaceCore;
        Arca::Index<Core> core;
    };

    template<class Derived>
    void Surface<Derived>::RenderStaged()
    {
        auto graphicsManager = Arca::Postulate<GraphicsManager*>(Owner()).Get();
        graphicsManager->RenderStaged(*Data());
    }

    template<class Derived>
    void Surface<Derived>::FullColor(const Color& color)
    {
        Data()->FullColor(color);
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
        core(init.template Create<Core>(std::move(data)))
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
}