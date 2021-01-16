#pragma once

namespace Atmos
{
    class EngineProvider;

    namespace World
    {
        class Manager;
    }

    namespace Window
    {
        class WindowBase;
    }

    class EngineExecution
    {
    public:
        EngineExecution(World::Manager& worldManager, Window::WindowBase& window);

        void Start();
    private:
        Window::WindowBase* window;

        World::Manager* worldManager;
    private:
        bool wasFocusedLastFrame = true;
    private:
        bool StartFrame();
        [[nodiscard]] bool IsCurrentlyFocused() const;

        void OnFocusLost();
        void OnFocusRegain();
    };
}