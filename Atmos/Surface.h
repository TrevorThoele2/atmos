#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "SurfaceCore.h"

#include "Color.h"

namespace Atmos::Render
{
    struct MaterialRender;
    struct CanvasRender;
    struct Line;

    template<class Derived>
    class Surface : public Arca::ClosedTypedRelic<Derived>
    {
    private:
        using BaseT = Arca::ClosedTypedRelic<Derived>;
    protected:
        using Init = typename BaseT::Init;
    public:
        void StageRender(const MaterialRender& materialRender);
        void StageRender(const CanvasRender& canvasRender);
        void StageRender(const Line& line);
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
    private:
        using Core = SurfaceCore;
        Arca::ShardIndex<Core> core;
    };

    template<class Derived>
    void Surface<Derived>::StageRender(const MaterialRender& materialRender)
    {
        Data()->StageRender(materialRender);
    }

    template<class Derived>
    void Surface<Derived>::StageRender(const CanvasRender& canvasRender)
    {
        Data()->StageRender(canvasRender);
    }

    template<class Derived>
    void Surface<Derived>::StageRender(const Line& line)
    {
        Data()->StageRender(line);
    }

    template<class Derived>
    void Surface<Derived>::RenderStaged()
    {

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
        core(init.owner.template Do<Arca::Create<Core>>(init.id, std::move(data)))
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