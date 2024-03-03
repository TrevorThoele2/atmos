#include "TypeRegistration.h"

#include "StaticMaterialView.h"
#include "DynamicMaterialView.h"
#include "MaterialViewCurator.h"
#include "LineCurator.h"

#include "DebugStatistics.h"
#include "TimeSettings.h"
#include "Camera.h"
#include "CameraCurator.h"
#include "FrameStopwatch.h"
#include "StopwatchStatistics.h"

#include "UniqueProviderRelic.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"

namespace Atmos
{
    void RegisterGlobalTypes(
        Arca::ReliquaryOrigin& origin,
        std::unique_ptr<Input::Manager>&& input,
        std::unique_ptr<Render::GraphicsManager>&& graphics,
        std::unique_ptr<Audio::AudioManager>&& audio)
    {
        origin
            .Register<Debug::Statistics>()
            .Register<Time::Information>()
            .Register<Time::Settings>()
            .Register<Time::StopwatchCore>()
            .Register<Time::StopwatchStatistics>()
            .Register<Time::RealStopwatch>()
            .Register<Time::FrameStopwatch>();

        RegisterProviderComputation<UniqueProviderRelic<Input::Manager>>(origin, std::move(input));
        RegisterProviderComputation<UniqueProviderRelic<Render::GraphicsManager>>(origin, std::move(graphics));
        RegisterProviderComputation<UniqueProviderRelic<Audio::AudioManager>>(origin, std::move(audio));
    }

    void RegisterFieldTypes(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary)
    {
        origin
            .Register<Bounds>()
            .Register<Render::MaterialViewCore>()
            .Register<Render::StaticMaterialView>()
            .Register<Render::DynamicMaterialView>()
            .Register<Render::MaterialViewCurator>()
            .Register<Render::Line>()
            .Register<Render::LineCurator>()
            .Register<Render::Camera>()
            .Register<Render::CameraCurator>();

        RegisterRedirectionComputation<UniqueProviderRelic<Input::Manager>>(origin, globalReliquary);
        RegisterRedirectionComputation<UniqueProviderRelic<Render::GraphicsManager>>(origin, globalReliquary);
        RegisterRedirectionComputation<UniqueProviderRelic<Audio::AudioManager>>(origin, globalReliquary);
    }
}
