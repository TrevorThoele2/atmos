#pragma once

#include "EngineInitializationProperties.h"
#include "EngineExecution.h"

#include <Arca/Reliquary.h>
#include <Arca/ReliquaryOrigin.h>
#include "WorldManager.h"

namespace Atmos
{
    class Engine
    {
    public:
        virtual ~Engine() = 0;

        void Setup();

        void UseField(World::Field&& field);
        void LoadWorld(const File::Path& filePath);

        void StartExecution();

        void Exit();
    protected:
        using InitializationProperties = EngineInitializationProperties;
    protected:
        Engine() = default;

        virtual InitializationProperties CreateInitializationProperties(Arca::Reliquary& reliquary) = 0;

        virtual void DoExit() = 0;
    private:
        [[nodiscard]] bool IsSetup() const;
        void SetupRequired() const;
    private:
        std::unique_ptr<Arca::Reliquary> globalReliquary;

        class ExecutionContext
        {
        public:
            EngineExecution execution;
            World::WorldManager worldManager;
        public:
            ExecutionContext(Engine& owner, World::WorldManager&& worldManager);
        };

        using ExecutionContextPtr = std::unique_ptr<ExecutionContext>;
        ExecutionContextPtr executionContext;

        void ProvideInitializationProperties(InitializationProperties&& properties);
    };
}