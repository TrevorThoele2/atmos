#pragma once

#include <memory>
#include "ScreenDimensions.h"

namespace Atmos::Render
{
    class GraphicsManager;

    class Surface
    {
    public:
        class Data
        {
        public:
            virtual ~Data() = 0;
            [[nodiscard]] Surface* Owner() const;
            virtual void SetAsRenderTarget() = 0;
            virtual void Present() = 0;
            virtual void Reset() = 0;
            virtual void Release() = 0;
            virtual ScreenDimensions Size() = 0;
        private:
            Surface* owner = nullptr;
        private:
            friend Surface;
        };

        using DataPtr = std::unique_ptr<Data>;
    public:
        explicit Surface(DataPtr&& data);
        Surface(Surface&& arg) noexcept;

        Surface& operator=(Surface&& arg) noexcept;

        [[nodiscard]] Data* GetData() const;
        template<class DataT>
        [[nodiscard]] DataT* GetData() const;

        void Present();

        void Reset();
        void Release();

        void FitToWindow();

        [[nodiscard]] ScreenDimensions Size() const;
    private:
        DataPtr data;
        ScreenDimensions size;
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