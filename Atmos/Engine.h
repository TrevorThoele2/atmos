#pragma once

#include "EngineInitializationProperties.h"
#include "EngineExecution.h"

#include "WorldManager.h"
#include "Logger.h"
#include "LoadAssetsUserContext.h"

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

        Engine(InitializationProperties&& initializationProperties, Logging::Logger& logger);
        Engine(Engine&& arg) noexcept = default;
    private:
        using Execution = EngineExecution;
        using ExecutionPtr = std::unique_ptr<Execution>;
        ExecutionPtr execution;

        void ChangeField(World::FieldID fieldID);

        [[nodiscard]] std::unique_ptr<Arca::Reliquary> CreateReliquary();
        [[nodiscard]] std::unique_ptr<Inscription::LoadAssetsUserContext> CreateLoadAssetsUserContext();
    private:
        struct Managers
        {
            std::unique_ptr<System::System> system;
            std::unique_ptr<Asset::Resource::Manager> assetResourceManager;
            std::unique_ptr<Window::WindowBase> window;
            std::unique_ptr<Audio::Manager> audio;
            std::unique_ptr<Input::Manager> input;
            std::unique_ptr<Render::GraphicsManager> graphics;
            std::unique_ptr<Render::TextManager> text;
            std::unique_ptr<Scripting::Manager> scripts;
            std::unique_ptr<World::Manager> world;
        } managers;
    private:
        File::Path assetsFilePath;
    private:
        Logging::Logger* logger;
    };
}