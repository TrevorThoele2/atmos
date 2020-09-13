#include "WindowProvider.h"

#include "NullWindow.h"

namespace Atmos::Window
{
    WindowProvider::WindowProvider()
    {
        value = std::make_unique<NullWindow>();
    }

    void WindowProvider::Setup(std::unique_ptr<WindowBase>&& value)
    {
        this->value = std::move(value);
        this->value->Setup();
    }

    WindowBase& WindowProvider::operator*()
    {
        return *value.get();
    }

    const WindowBase& WindowProvider::operator*() const
    {
        return *value.get();
    }

    WindowBase* WindowProvider::operator->()
    {
        return value.get();
    }

    const WindowBase* WindowProvider::operator->() const
    {
        return value.get();
    }

    WindowProvider window;
}