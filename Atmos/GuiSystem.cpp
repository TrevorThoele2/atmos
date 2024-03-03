
#include "GuiSystem.h"

#include "ObjectManager.h"
#include "InitializationFileSystem.h"
#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

#include <AGUI/System.h>

namespace Atmos
{
    GuiSystem::GuiSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    GuiSystem::GuiSystem(const ::Inscription::Table<GuiSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
    {}

    void GuiSystem::InitializeImpl()
    {
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
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::GuiSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::GuiSystem, "GuiSystem");
}