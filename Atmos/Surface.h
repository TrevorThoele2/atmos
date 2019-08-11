#pragma once

#include <memory>
#include "ScreenDimensions.h"

namespace Atmos::Render
{
    class GraphicsManager;

    class Surface
    {
    public:
        typedef ScreenDimensions::Dimension Dimension;
        typedef ScreenDimensions Size;
    public:
        class Data
        {
        public:
            typedef Surface::Size Size;
        public:
            virtual ~Data() = 0;
            Surface* GetOwner() const;
            virtual void SetAsRenderTarget() = 0;
            virtual void Present() = 0;
            virtual void Reset() = 0;
            virtual void Release() = 0;
            virtual Size GetSize() = 0;
        private:
            Surface* owner;
        private:
            friend Surface;
        };

        typedef std::unique_ptr<Data> DataPtr;
    public:
        Surface(DataPtr&& data);
        Surface(Surface&& arg);

        Surface& operator=(Surface&& arg);

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
    DataT* Surface::GetData() const
    {
        return static_cast<DataT*>(data.get());
    }
}