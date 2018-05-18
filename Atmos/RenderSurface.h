#pragma once

#include <memory>
#include "Join.h"

namespace Atmos
{
    class GraphicsHandlerBase;
    class RenderSurface
    {
    public:
        typedef unsigned int Dimension;
        typedef Join2<Dimension> ScreenDimensions;

        class Data
        {
        private:
            RenderSurface *owner;
            friend RenderSurface;
        public:
            virtual ~Data() = 0 {}
            RenderSurface* GetOwner() const;
            virtual void SetAsRenderTarget() = 0;
            virtual void Present() = 0;
            virtual void Reset() = 0;
            virtual void Release() = 0;
            virtual ScreenDimensions GetDimensions() = 0;
        };
    private:
        std::unique_ptr<Data> data;
        ScreenDimensions dimensions;

        void SetData(Data *set);
        void SetData(std::unique_ptr<Data> &&set);

        void SetupDimensions();

        void SetAsRenderTargetImpl();
        friend GraphicsHandlerBase;
    public:
        RenderSurface(Data *data);
        RenderSurface(RenderSurface &&arg);
        RenderSurface& operator=(RenderSurface &&arg);

        Data* GetData() const;
        template<class DataT>
        DataT* GetData() const;

        void DestroyThis();

        void SetAsRenderTarget();
        void Present();

        void Reset();
        void Release();

        void FitToWindow();

        const ScreenDimensions& GetDimensions() const;
    };

    template<class DataT>
    DataT* RenderSurface::GetData() const
    {
        return static_cast<DataT*>(data.get());
    }
}