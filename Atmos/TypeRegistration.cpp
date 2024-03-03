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

#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"

namespace Atmos
{
    void RegisterFieldTypes(
        Arca::ReliquaryOrigin& origin,
        Audio::AudioManager& audio,
        Input::Manager& input,
        Render::GraphicsManager& graphics)
    {
        Audio::RegisterTypes(origin, audio);
        Input::RegisterTypes(origin, input);
        Render::RegisterTypes(origin, graphics);
        Asset::RegisterTypes(origin);
        Time::RegisterTypes(origin);
        Debug::RegisterTypes(origin);
    }

    namespace File
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Curator>();
        }
    }

    namespace Input
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin, Manager& manager)
        {
            
        }
    }

    namespace Render
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin, GraphicsManager& manager)
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
                .Register<GraphicsCurator>(std::ref(manager))
                .Register<SurfaceCore>()
                .Register<MainSurface>()
                .Register<AncillarySurface>()
                .Register<SurfaceCurator>();
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
        void RegisterTypes(Arca::ReliquaryOrigin& origin, AudioManager& manager)
        {
            
        }
    }

    namespace Asset
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Core>()
                .Register<AudioAsset>()
                .Register<Image>()
                .Register<Material>()
                .Register<Script>()
                .Register<Shader>()
                .Register<Mapped<AudioAsset>>()
                .Register<Mapped<Image>>()
                .Register<Mapped<Material>>()
                .Register<Mapped<Script>>()
                .Register<Mapped<Shader>>()
                .Register<AudioAssetCurator>()
                .Register<ImageCurator>()
                .Register<MaterialCurator>()
                .Register<ScriptCurator>()
                .Register<ShaderCurator>();
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
