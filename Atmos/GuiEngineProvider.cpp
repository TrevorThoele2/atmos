#include "GuiEngineProvider.h"

namespace Atmos
{
    void GuiEngineProvider::PostConstruct()
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

        auto debugStatistics = Manager()->FindSystem<StatisticsSystem>();
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

namespace Arca
{
    const TypeName Traits<Atmos::GuiEngineProvider>::typeName = "GuiEngineProvider";
}