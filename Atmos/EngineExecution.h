#pragma once

namespace Arca
{
    class Reliquary;
}

namespace Atmos
{
    class EngineProvider;

    namespace Window
    {
        class WindowProvider;
    }

    namespace World
    {
        class WorldManager;
    }

    class EngineExecution
    {
    public:
        EngineExecution(Arca::Reliquary& globalReliquary, World::WorldManager& worldManager);

        void Start();
    private:
        Arca::Reliquary* globalReliquary;

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