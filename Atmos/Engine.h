#pragma once

#include "CreatedEngineContext.h"
#include "EngineExecution.h"

#include "WorldManager.h"
#include "ObjectManager.h"

#include "ObjectRegistration.h"

namespace Atmos
{
    class Engine
    {
    public:
        virtual ~Engine() = 0;

        void Setup();

        void LoadWorld(const FilePath& filePath);

        void StartExecution();

        void Exit();
    protected:
        typedef CreatedEngineContext CreatedContext;
    protected:
        WorldManager worldManager;

        ObjectManager globalObjectManager;
    protected:
        Engine();

        virtual CreatedContext CreateDefaultContext() = 0;

        virtual void DoExit() = 0;
    private:
        EngineExecution execution;
        ObjectRegistration registration;
    };
}