#include "TypeRegistration.h"

#include "DebugStatistics.h"
#include "TimeCurator.h"
#include "StaticMaterialView.h"
#include "DynamicMaterialView.h"
#include "Camera.h"
#include "FrameStopwatch.h"
#include "MaterialViewCurator.h"

#include "UniqueProviderRelic.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"

namespace Atmos
{
    void RegisterGlobalTypes(Arca::ReliquaryOrigin& origin)
    {
        origin
            .Register<Debug::Statistics>()
            .Register<Time::TimeCurator>()
            .Register<Time::Information>()
            .Register<Time::Settings>()
            .Register<Time::RealStopwatch>()
            .Register<Time::FrameStopwatch>();

        RegisterProviderComputation<UniqueProviderRelic<Input::Manager>>(origin);
        RegisterProviderComputation<UniqueProviderRelic<Render::GraphicsManager>>(origin);
        RegisterProviderComputation<UniqueProviderRelic<Audio::AudioManager>>(origin);
    }

    void RegisterFieldTypes(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary)
    {
        origin
            .Register<Bounds>()
            .Register<Render::MaterialViewCore>()
            .Register<Render::StaticMaterialView>()
            .Register<Render::DynamicMaterialView>()
            .Register<Render::MaterialViewCurator>()
            .Register<Render::Camera>();

        RegisterRedirectionComputation<UniqueProviderRelic<Input::Manager>>(origin, globalReliquary);
        RegisterRedirectionComputation<UniqueProviderRelic<Render::GraphicsManager>>(origin, globalReliquary);
        RegisterRedirectionComputation<UniqueProviderRelic<Audio::AudioManager>>(origin, globalReliquary);
    }
}
