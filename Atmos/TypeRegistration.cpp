#include "TypeRegistration.h"

#include "StaticMaterialView.h"
#include "DynamicMaterialView.h"
#include "MaterialViewCurator.h"
#include "LineCurator.h"
#include "RenderCurator.h"
#include "Camera.h"
#include "CameraCurator.h"

#include "AudioAsset.h"
#include "ImageAsset.h"
#include "MaterialAsset.h"
#include "ScriptAsset.h"
#include "ShaderAsset.h"
#include "MappedAssets.h"

#include "TimeSettings.h"
#include "FrameStopwatch.h"
#include "StopwatchStatistics.h"
#include "StopwatchCurator.h"

#include "DebugStatistics.h"

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
        Time::RegisterTypes(origin);
        Debug::RegisterTypes(origin);

        RegisterProviderPostulate<UniqueProviderRelic<Input::Manager>>(origin, std::move(input));
        RegisterProviderPostulate<UniqueProviderRelic<Render::GraphicsManager>>(origin, std::move(graphics));
        RegisterProviderPostulate<UniqueProviderRelic<Audio::AudioManager>>(origin, std::move(audio));
    }

    void RegisterFieldTypes(
        Arca::ReliquaryOrigin& origin,
        Arca::Reliquary& globalReliquary)
    {
        Render::RegisterTypes(origin, *Arca::Postulate<Render::GraphicsManager*>(globalReliquary));
        Time::RegisterTypes(origin);

        Input::RegisterGlobalRedirectionTypes(origin, globalReliquary);
        Render::RegisterGlobalRedirectionTypes(origin, globalReliquary);
        Audio::RegisterGlobalRedirectionTypes(origin, globalReliquary);
    }

    namespace Input
    {
        void RegisterGlobalRedirectionTypes(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary)
        {
            RegisterRedirectionPostulate<UniqueProviderRelic<Manager>>(origin, globalReliquary);
        }
    }

    namespace Render
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin, GraphicsManager& graphicsManager)
        {
            origin
                .Register<Bounds>()
                .Register<MaterialViewCore>()
                .Register<StaticMaterialView>()
                .Register<DynamicMaterialView>()
                .Register<MaterialViewCurator>()
                .Register<Line>()
                .Register<LineCurator>()
                .Register<Camera>()
                .Register<CameraCurator>()
                .Register<Curator>()
                .Register<SurfaceCore>()
                .Register<MainSurface>(graphicsManager.CreateMainSurfaceData());
        }

        void RegisterGlobalRedirectionTypes(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary)
        {
            RegisterRedirectionPostulate<UniqueProviderRelic<GraphicsManager>>(origin, globalReliquary);
        }

        Arca::Stage Stage()
        {
            Arca::Stage stage;
            stage.Add<MaterialViewCurator>();
            stage.Add<LineCurator>();
            stage.Add<CameraCurator>();
            stage.Add<Curator>();
            return stage;
        }
    }

    namespace Audio
    {
        void RegisterGlobalRedirectionTypes(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary)
        {
            RegisterRedirectionPostulate<UniqueProviderRelic<AudioManager>>(origin, globalReliquary);
        }
    }

    namespace Asset
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Core>()
                .Register<AudioAsset>()
                .Register<ImageAsset>()
                .Register<MaterialAsset>()
                .Register<ScriptAsset>()
                .Register<ShaderAsset>()
                .Register<MappedAssets<AudioAsset>>()
                .Register<MappedAssets<ImageAsset>>()
                .Register<MappedAssets<MaterialAsset>>()
                .Register<MappedAssets<ScriptAsset>>()
                .Register<MappedAssets<ShaderAsset>>();
        }
    }

    namespace Time
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Information>()
                .Register<Settings>()
                .Register<StopwatchCore>()
                .Register<StopwatchStatistics>()
                .Register<RealStopwatch>()
                .Register<FrameStopwatch>()
                .Register<StopwatchCurator>();
        }
    }

    namespace Debug
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Statistics>();
        }

        void RegisterGlobalRedirectionTypes(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary)
        {
            origin.Postulate<const Statistics*>(
                [&globalReliquary](Arca::Reliquary&) -> const Statistics*
                {
                    const Arca::Index<Statistics> backing(globalReliquary);
                    return backing;
                });
        }
    }
}
