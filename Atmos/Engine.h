#pragma once

#include "EngineInitializationProperties.h"
#include "EngineExecution.h"

#include "WorldManager.h"

#include "Logger.h"

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

        [[nodiscard]] Logging::Logger& Logger();
    protected:
        using InitializationProperties = EngineInitializationProperties;
    protected:
        Engine(Logging::Logger& logger);

        virtual InitializationProperties CreateInitializationProperties(Logging::Logger& logger) = 0;
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
            ExecutionContext(World::WorldManager&& worldManager, Window::WindowBase& window);
        };

        using ExecutionContextPtr = std::unique_ptr<ExecutionContext>;
        ExecutionContextPtr executionContext;

        void ChangeField(World::FieldID fieldID, const File::Path& assetsFilePath);
    private:
        struct Managers
        {
            std::unique_ptr<Asset::ImageManager> imageAssetManager;
            std::unique_ptr<Window::WindowBase> window;
            std::unique_ptr<Audio::AudioManager> audio;
            std::unique_ptr<Input::Manager> input;
            std::unique_ptr<Render::GraphicsManager> graphics;
            std::unique_ptr<Scripting::Manager> scripts;
        };
        Managers managers;
    private:
        Logging::Logger* logger;
    };
}