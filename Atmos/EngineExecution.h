#pragma once

#include "Event.h"

namespace Atmos
{
    class ObjectManager;
    class EngineSystem;

    namespace Window
    {
        class WindowSystem;
    }

    namespace World
    {
        class WorldManager;
    }

    class EngineExecution
    {
    public:
        EngineExecution(ObjectManager& globalObjectManager, World::WorldManager& worldManager);

        void Start();
    private:
        ObjectManager* globalObjectManager;
        EngineSystem* engineSystem;

        World::WorldManager* worldManager;
    private:
        bool wasFocusedLastPass;
        bool isFocusLost;
    private:
        bool StartFrame(Window::WindowSystem* windowSystem);
        bool IsCurrentlyFocused(Window::WindowSystem* windowSystem) const;

        void OnFocusLost();
        void OnFocusRegain();
    };
}