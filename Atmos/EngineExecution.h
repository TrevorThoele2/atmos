#pragma once

namespace Atmos
{
    class EngineProvider;

    namespace World
    {
        class WorldManager;
    }

    namespace Window
    {
        class WindowBase;
    }

    class EngineExecution
    {
    public:
        EngineExecution(World::WorldManager& worldManager, Window::WindowBase& window);

        void Start();
    private:
        Window::WindowBase* window;

        World::WorldManager* worldManager;
    private:
        bool wasFocusedLastPass = false;
        bool isFocusLost = false;
    private:
        bool StartFrame();
        [[nodiscard]] bool IsCurrentlyFocused() const;

        void OnFocusLost();
        void OnFocusRegain();
    };
}