
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

namespace Inscription
{
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_SERIALIZE_FUNCTION(::Atmos::GuiSystem)
    {
        if (scribe.IsOutput())
        {
            auto& outputScribe = *scribe.AsOutput();

            outputScribe.SaveUnowningPointer(obj.Manager());
        }
    }

    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_CONSTRUCT_OBJECT_FUNCTION(::Atmos::GuiSystem)
    {
        auto& inputScribe = *scribe.AsInput();

        ::Atmos::ObjectManager* objectManager;
        inputScribe.LoadUnowningPointer(objectManager);
        obj = new ManagedT(*objectManager);
    }

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::GuiSystem, "GuiSystem");
}