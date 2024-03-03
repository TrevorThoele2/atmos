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

#include "UniqueProviderRelic.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"

namespace Atmos
{
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

        RegisterProviderComputation<UniqueProviderRelic<Input::Manager>>(origin);
        RegisterProviderComputation<UniqueProviderRelic<Render::GraphicsManager>>(origin);
        RegisterProviderComputation<UniqueProviderRelic<Audio::AudioManager>>(origin);
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

        RegisterRedirectionComputation<UniqueProviderRelic<Input::Manager>>(origin, globalReliquary);
        RegisterRedirectionComputation<UniqueProviderRelic<Render::GraphicsManager>>(origin, globalReliquary);
        RegisterRedirectionComputation<UniqueProviderRelic<Audio::AudioManager>>(origin, globalReliquary);
    }
}
