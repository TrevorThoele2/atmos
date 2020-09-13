#pragma once

#include <memory>
#include "WindowBase.h"

namespace Atmos::Window
{
    class WindowProvider final
    {
    public:
        WindowProvider();

        template<class T, class... Args>
        void Setup(Args&& ... args);
        void Setup(std::unique_ptr<WindowBase>&& value);

        WindowBase& operator*();
        const WindowBase& operator*() const;
        WindowBase* operator->();
        const WindowBase* operator->() const;
    private:
        using Value = WindowBase;
        using ValuePtr = std::unique_ptr<Value>;
        ValuePtr value;
    };

    template<class T, class... Args>
    void WindowProvider::Setup(Args&& ... args)
    {
        Setup(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
    }

    extern WindowProvider window;
}