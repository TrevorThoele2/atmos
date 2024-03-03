
#include "GuiSystem.h"

#include "ObjectManager.h"
#include "InitializationFileSystem.h"
#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos
{
    GuiSystem::GuiSystem(ObjectManager& manager) : UniqueProviderSystem(manager)
    {}

    void GuiSystem::InitializeImpl()
    {
        /*
        auto initialization = Manager()->FindSystem<InitializationFileSystem>();
        auto entry = initialization->graphics.CreateEntry<::Agui::Resolution::Size>("Windowed", [this]()
        {
            return ::Agui::System::GetCurrentResolution()->GetSize();
        });
        entry->onLoaded.Subscribe([this](::Agui::Resolution::Size value) {
            ::Agui::System::SetRes(value);
        });

        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->windowPage.resolutionWidth.retrievalFunction = [this]() -> String
        {
            return ToString(::Agui::System::GetCurrentResolution()->GetWidth());
        };
        debugStatistics->windowPage.resolutionHeight.retrievalFunction = [this]() -> String
        {
            return ToString(::Agui::System::GetCurrentResolution()->GetHeight());
        };
        */
    }
}