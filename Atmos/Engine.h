#pragma once

#include "EngineInitializationProperties.h"
#include "EngineExecution.h"

#include "WorldManager.h"
#include "ObjectManager.h"
#include "ObjectManagerFactory.h"

#include "TypeRegistration.h"

namespace Atmos
{
    class Engine
    {
    public:
        virtual ~Engine() = 0;

        void Setup();

        void LoadWorld(const File::Path& filePath);

        void StartExecution();

        void Exit();
    protected:
        typedef EngineInitializationProperties InitializationProperties;
    protected:
        Engine();

        virtual InitializationProperties CreateInitializationProperties(ObjectManager& globalObjectManager) = 0;

        virtual void DoExit() = 0;
    private:
        bool IsSetup() const;

        void SetupRequired();
    private:
        ObjectManager globalObjectManager;
        ObjectManagerFactory localObjectManagerFactory;

        TypeRegistration typeRegistration;
        TypeRegistration::Group* globalTypes;
        TypeRegistration::Group* localTypes;
        TypeRegistration::Group* infrastructureTypes;

        class ExecutionContext
        {
        public:
            EngineExecution execution;
            World::WorldManager worldManager;
        public:
            ExecutionContext(Engine& owner, World::WorldManager&& worldManager);
        };

        typedef std::unique_ptr<ExecutionContext> ExecutionContextPtr;
        ExecutionContextPtr executionContext;
    };
}