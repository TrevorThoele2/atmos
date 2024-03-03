#include "WindowSystem.h"

#include "NullWindow.h"

#include "ObjectManager.h"
#include "InitializationFileSystem.h"
#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos::Window
{
    WindowSystem::WindowSystem(ObjectManager& manager) :
        UniqueProviderSystem(manager, UniqueProviderSystem::ValuePtr(new NullWindow(manager)))
    {}

    void WindowSystem::InitializeImpl()
    {
        auto initialization = Manager()->FindSystem<InitializationFileSystem>();
        auto entry = initialization->graphics.CreateEntry<bool>("Fullscreen", [this]() -> bool
        {
            return this->Get()->IsFullscreen();
        });
        entry->onLoaded.Subscribe([this](bool value) {
            this->Get()->SetFullscreen(value);
        });

        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->windowPage.windowWidth.retrievalFunction = [this]() -> String
        {
            return ToString(Get()->WindowSize().width);
        };
        debugStatistics->windowPage.windowHeight.retrievalFunction = [this]() -> String
        {
            return ToString(Get()->WindowSize().height);
        };
        debugStatistics->windowPage.clientWidth.retrievalFunction = [this]() -> String
        {
            return ToString(Get()->ClientSize().width);
        };
        debugStatistics->windowPage.clientHeight.retrievalFunction = [this]() -> String
        {
            return ToString(Get()->ClientSize().height);
        };
        debugStatistics->windowPage.startX.retrievalFunction = [this]() -> String
        {
            return ToString(Get()->StartPosition().x);
        };
        debugStatistics->windowPage.startY.retrievalFunction = [this]() -> String
        {
            return ToString(Get()->StartPosition().y);
        };
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Window::WindowSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::UniqueProviderSystem<::Atmos::Window::WindowBase>>(object, archive);
    }
}