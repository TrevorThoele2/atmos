#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "SurfaceCore.h"

#include "Camera.h"

namespace Atmos::Render
{
    struct ImageRender;
    struct LineRender;

    template<class Derived>
    class Surface : public Arca::ClosedTypedRelic<Derived>
    {
    private:
        using BaseT = Arca::ClosedTypedRelic<Derived>;
    protected:
        using Init = typename BaseT::Init;
    public:
        using DataT = SurfaceData;
    public:
        virtual ~Surface() = 0;

        void StageRender(const ImageRender& imageRender) const;
        void StageRender(const LineRender& lineRender) const;

        void DrawFrame() const;

        [[nodiscard]] ScreenSize Size() const;

        [[nodiscard]] DataT* Data() const;
        template<class DataT>
        [[nodiscard]] DataT* Data() const;
    protected:
        using DataPtr = std::unique_ptr<DataT>;
    protected:
        Surface(Init init, DataPtr&& data);

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
        Data()->StageRender(imageRender);
    }

    template<class Derived>
    void Surface<Derived>::StageRender(const LineRender& lineRender) const
    {
        Data()->StageRender(lineRender);
    }

    template<class Derived>
    void Surface<Derived>::DrawFrame() const
    {
        Data()->DrawFrame(Core()->backgroundColor);
    }

    template<class Derived>
    ScreenSize Surface<Derived>::Size() const
    {
        return Data()->Size();
    }

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
    Surface<Derived>::Surface(Init init, DataPtr&& data) :
        Arca::ClosedTypedRelic<Derived>(init),
        core(init.template Create<SurfaceCore>(std::move(data)))
    {}

    template<class Derived>
    Arca::Index<SurfaceCore> Surface<Derived>::Core() const
    {
        return core;
    }
}