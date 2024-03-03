#include "TypeRegistration.h"

#include "FileCurator.h"

#include "StaticImage.h"
#include "DynamicImage.h"
#include "ImageCurator.h"
#include "LineCurator.h"
#include "RenderCurator.h"
#include "GraphicsCurator.h"
#include "Camera.h"
#include "CameraCurator.h"
#include "AncillarySurface.h"
#include "SurfaceCurator.h"

#include "AudioAsset.h"
#include "ImageAsset.h"
#include "MaterialAsset.h"
#include "ScriptAsset.h"
#include "ShaderAsset.h"
#include "MappedAssets.h"
#include "AudioAssetCurator.h"
#include "ImageAssetCurator.h"
#include "MaterialAssetCurator.h"
#include "ScriptAssetCurator.h"
#include "ShaderAssetCurator.h"

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
        Render::RegisterTypes(origin);
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

    namespace File
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Curator>();
        }
    }

    namespace Render
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Bounds>()
                .Register<ImageCore>()
                .Register<StaticImage>()
                .Register<DynamicImage>()
                .Register<ImageCurator>()
                .Register<Line>()
                .Register<LineCurator>()
                .Register<Camera>()
                .Register<CameraCurator>()
                .Register<Curator>()
                .Register<GraphicsCurator>()
                .Register<SurfaceCore>()
                .Register<MainSurface>()
                .Register<AncillarySurface>()
                .Register<SurfaceCurator>();
        }

        void RegisterGlobalRedirectionTypes(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary)
        {
            RegisterRedirectionPostulate<UniqueProviderRelic<GraphicsManager>>(origin, globalReliquary);
        }

        Arca::Stage Stage()
        {
            Arca::Stage stage;
            stage.Add<ImageCurator>();
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
                .Register<MappedAssets<ShaderAsset>>()
                .Register<AudioAssetCurator>()
                .Register<ImageAssetCurator>()
                .Register<MaterialAssetCurator>()
                .Register<ScriptAssetCurator>()
                .Register<ShaderAssetCurator>();
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
