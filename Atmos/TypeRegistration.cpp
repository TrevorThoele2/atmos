#include "TypeRegistration.h"

#include "DebugStatistics.h"
#include "FocusLost.h"
#include "FocusRegained.h"
#include "TimeCurator.h"
#include "StaticMaterialView.h"
#include "DynamicMaterialView.h"
#include "Camera.h"
#include "FrameStopwatch.h"
#include "MaterialViewCurator.h"

#include "UniqueProvider.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"

namespace Atmos
{
    template<class Provider>
    void RegisterProviderComputation(Arca::ReliquaryOrigin& origin)
    {
        using Value = typename Provider::Value;

        origin.Type<Provider>();
        origin.Compute<Value*>(
            [](Arca::Reliquary& reliquary) -> Value*
            {
                const Arca::GlobalPtr<Provider> backing(reliquary);
                return backing->Get();
            });
    }

    template<class Provider>
    void RegisterRedirectionComputation(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary)
    {
        using Value = typename Provider::Value;

        origin.Compute<Value*>(
            [&globalReliquary](Arca::Reliquary& reliquary) -> Value*
            {
                const Arca::GlobalPtr<Provider> backing(globalReliquary);
                return backing->Get();
            });
    }

    void RegisterCommonTypes(Arca::ReliquaryOrigin& origin)
    {
        origin
            .Type<FocusRegained>()
            .Type<FocusLost>();
    }

    void RegisterGlobalTypes(Arca::ReliquaryOrigin& origin)
    {
        RegisterCommonTypes(origin);

        origin
            .Type<Debug::Statistics>()
            .Type<Time::TimeCurator>()
            .Type<Time::Information>()
            .Type<Time::Settings>()
            .Type<Time::RealStopwatch>()
            .Type<Time::FrameStopwatch>();

        RegisterProviderComputation<UniqueProvider<Input::Manager>>(origin);
        RegisterProviderComputation<UniqueProvider<Render::GraphicsManager>>(origin);
        RegisterProviderComputation<UniqueProvider<Audio::AudioManager>>(origin);
    }

    void RegisterFieldTypes(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary)
    {
        RegisterCommonTypes(origin);

        origin
            .Type<Bounds>()
            .Type<Render::MaterialViewCore>()
            .Type<Render::StaticMaterialView>()
            .Type<Render::DynamicMaterialView>()
            .Type<Render::MaterialViewCurator>()
            .Type<Render::Camera>();

        RegisterRedirectionComputation<UniqueProvider<Input::Manager>>(origin, globalReliquary);
        RegisterRedirectionComputation<UniqueProvider<Render::GraphicsManager>>(origin, globalReliquary);
        RegisterRedirectionComputation<UniqueProvider<Audio::AudioManager>>(origin, globalReliquary);
    }
}
