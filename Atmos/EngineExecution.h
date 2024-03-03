#pragma once

#include "Event.h"

namespace Atmos
{
    class ObjectManager;
    class WorldManager;
    class WindowSystem;
    class EngineSystem;

    class EngineExecution
    {
    public:
        EngineExecution(ObjectManager& globalObjectManager, WorldManager& worldManager);

        void Start();
    private:
        ObjectManager* globalObjectManager;
        EngineSystem* engineSystem;

        WorldManager* worldManager;
    private:
        bool wasFocusedLastPass;
        bool isFocusLost;
    private:
        bool StartFrame(WindowSystem* windowSystem);
        bool IsCurrentlyFocused(WindowSystem* windowSystem) const;

        void OnFocusLost();
        void OnFocusRegain();
    };
}