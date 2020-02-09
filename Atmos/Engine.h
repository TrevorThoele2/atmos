#pragma once

#include "EngineInitializationProperties.h"
#include "EngineExecution.h"

#include <Arca/Reliquary.h>
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

        virtual void SetupImplementation() = 0;

        virtual InitializationProperties CreateInitializationProperties() = 0;

        virtual void DoExit() = 0;
    protected:
        [[nodiscard]] Arca::Reliquary* GlobalReliquary();
        [[nodiscard]] const Arca::Reliquary* GlobalReliquary() const;
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
    };
}