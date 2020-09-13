#pragma once

#include "EngineInitializationProperties.h"
#include "EngineExecution.h"

#include "WorldManager.h"

namespace Atmos
{
    class Engine
    {
    public:
        virtual ~Engine() = 0;

        void Setup();

        void UseField(World::Field&& field, const File::Path& assetsFilePath);
        void LoadWorld(const File::Path& filePath, const File::Path& assetsFilePath);
        World::Field* CurrentField();

        void StartExecution();

        void Exit();
    protected:
        using InitializationProperties = EngineInitializationProperties;
    protected:
        Engine() = default;

        virtual void SetupImplementation() = 0;

        virtual InitializationProperties CreateInitializationProperties() = 0;

        virtual void DoExit() = 0;
    private:
        [[nodiscard]] bool IsSetup() const;
        void SetupRequired() const;
    private:
        class ExecutionContext
        {
        public:
            EngineExecution execution;
            World::WorldManager worldManager;
        public:
            ExecutionContext(World::WorldManager&& worldManager);
        };

        using ExecutionContextPtr = std::unique_ptr<ExecutionContext>;
        ExecutionContextPtr executionContext;

        void ChangeField(World::FieldID fieldID, const File::Path& assetsFilePath);
    private:
        struct Managers
        {
            std::unique_ptr<Audio::AudioManager> audio;
            std::unique_ptr<Input::Manager> input;
            std::unique_ptr<Render::GraphicsManager> graphics;
        };
        Managers managers;
    };
}