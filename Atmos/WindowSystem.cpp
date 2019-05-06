
#include "WindowSystem.h"

#include "NullWindow.h"

#include "ObjectManager.h"
#include "InitializationFileSystem.h"
#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos
{
    WindowSystem::WindowSystem(ObjectManager& manager) :
        UniqueProviderSystem(manager, UniqueProviderSystem::ValuePtr(new NullWindow(manager)))
    {}

    WindowSystem::WindowSystem(const ::Inscription::Table<WindowSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(UniqueProviderSystem<WindowBase>)
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
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::WindowSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::UniqueProviderSystem<::Atmos::WindowBase>);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::WindowSystem, "WindowSystem");
}