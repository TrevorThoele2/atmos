#pragma once

#include <memory>
#include "ScreenDimensions.h"

namespace Atmos
{
    class GraphicsManager;

    class RenderSurface
    {
    public:
        typedef ScreenDimensions::Dimension Dimension;
        typedef ScreenDimensions Size;
    public:
        class Data
        {
        public:
            typedef RenderSurface::Size Size;
        public:
            virtual ~Data() = 0;
            RenderSurface* GetOwner() const;
            virtual void SetAsRenderTarget() = 0;
            virtual void Present() = 0;
            virtual void Reset() = 0;
            virtual void Release() = 0;
            virtual Size GetSize() = 0;
        private:
            RenderSurface* owner;
        private:
            friend RenderSurface;
        };

        typedef std::unique_ptr<Data> DataPtr;
    public:
        RenderSurface(DataPtr&& data);
        RenderSurface(RenderSurface&& arg);

        RenderSurface& operator=(RenderSurface&& arg);

        Data* GetData() const;
        template<class DataT>
        DataT* GetData() const;

        void Present();

        void Reset();
        void Release();

        void FitToWindow();

        const Size& GetSize() const;
    private:
        DataPtr data;
        Size size;
    private:
        void SetData(DataPtr&& set);

        void SetupDimensions();
        void SetAsRenderTargetImpl();
    private:
        friend GraphicsManager;
    };

    template<class DataT>
    DataT* RenderSurface::GetData() const
    {
        return static_cast<DataT*>(data.get());
    }
}