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

        void UseField(World::Field&& field, std::vector<Property>&& worldProperties, const File::Path& assetsFilePath);
        void LoadWorld(const File::Path& filePath, const File::Path& assetsFilePath);
        World::Field* CurrentField();

        void StartExecution();

        [[nodiscard]] Logging::Logger& Logger();
    protected:
        using InitializationProperties = EngineInitializationProperties;

        template<class WorldManagerT>
        Engine(
            InitializationProperties&& initializationProperties,
            Chroma::TypeIdentity<WorldManagerT> worldManagerType,
            Logging::Logger& logger);
        Engine(Engine&& arg) noexcept = default;

        [[nodiscard]] World::Manager* WorldManager() const;
    private:
        using Execution = EngineExecution;
        using ExecutionPtr = std::unique_ptr<Execution>;
        ExecutionPtr execution;

        void ChangeField(World::FieldID fieldID);
    private:
        struct Managers
        {
            std::unique_ptr<System::System> system;
            std::unique_ptr<Asset::Resource::Manager> assetResource;
            std::unique_ptr<Window::WindowBase> window;
            std::unique_ptr<Audio::Manager> audio;
            std::unique_ptr<Input::Manager> input;
            std::unique_ptr<Render::GraphicsManager> graphics;
            std::unique_ptr<Render::TextManager> text;
            std::unique_ptr<Scripting::Manager> scripting;
            std::unique_ptr<World::Manager> world;
        } managers;

        [[nodiscard]] World::FieldInitialization CreateFieldInitialization() const;
    private:
        File::Path assetsFilePath;
    private:
        Logging::Logger* logger;
    };

    template<class WorldManagerT>
    Engine::Engine(
        InitializationProperties&& initializationProperties,
        Chroma::TypeIdentity<WorldManagerT>,
        Logging::Logger& logger)
        :
        managers
        {
            std::move(initializationProperties.system),
            std::move(initializationProperties.assetResourceManager),
            std::move(initializationProperties.window),
            std::move(initializationProperties.audioManager),
            std::move(initializationProperties.inputManager),
            std::move(initializationProperties.graphicsManager),
            std::move(initializationProperties.textManager),
            std::move(initializationProperties.scriptManager),
            std::make_unique<WorldManagerT>([this]() { return CreateFieldInitialization(); })
        },
        logger(&logger)
    {
        managers.window->ChangeSize(Spatial::Size2D{ 1024, 768 });
        managers.window->CenterOnScreen();

        execution = std::make_unique<Execution>(*managers.world, *managers.window);
    }
}