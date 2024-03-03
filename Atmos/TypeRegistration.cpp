#include "TypeRegistration.h"

#include "BoundsCurator.h"
#include "RelativeBounds.h"

#include "FileCurator.h"

#include "StaticImage.h"
#include "DynamicImage.h"
#include "RelativeImage.h"
#include "ImageCurator.h"
#include "LineCurator.h"
#include "GridRegion.h"
#include "GridRegionCurator.h"
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

#include "Entity.h"
#include "EntityCurator.h"
#include "EntityPrototype.h"
#include "MappedEntities.h"

#include "TimeSettings.h"
#include "FrameStopwatch.h"
#include "StopwatchStatistics.h"
#include "StopwatchCurator.h"
#include "FrameStartCurator.h"
#include "FrameEndCurator.h"

#include "LoggingCurator.h"

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
        Spatial::RegisterTypes(origin);
        Time::RegisterTypes(origin);
        Logging::RegisterTypes(origin);
        Debug::RegisterTypes(origin);
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
                .Register<ImageCore>()
                .Register<StaticImage>()
                .Register<DynamicImage>()
                .Register<RelativeImage>()
                .Register<ImageCurator>()
                .Register<Line>()
                .Register<LineCurator>()
                .Register<GridRegion>()
                .Register<GridRegionCurator>()
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
            stage.Add<GridRegionCurator>();
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

    namespace Spatial
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Bounds>()
                .Register<BoundsCurator>()
                .Register<RelativeBounds>();
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

    namespace Asset
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Core>()
                .Register<Audio>()
                .Register<Image>()
                .Register<Material>()
                .Register<Script>()
                .Register<Shader>()
                .Register<Mapped<Audio>>()
                .Register<Mapped<Image>>()
                .Register<Mapped<Material>>()
                .Register<Mapped<Script>>()
                .Register<Mapped<Shader>>()
                .Register<AudioCurator>()
                .Register<ImageCurator>()
                .Register<MaterialCurator>()
                .Register<ScriptCurator>()
                .Register<ShaderCurator>();
        }
    }

    namespace Entity
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Entity>()
                .Register<Curator>()
                .Register<Prototype>()
                .Register<MappedEntities>();
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
                .Register<StopwatchCurator>()
                .Register<FrameStartCurator>()
                .Register<FrameEndCurator>();
        }

        Arca::Stage StartStage()
        {
            Arca::Stage stage;
            stage.Add<FrameStartCurator>();
            return stage;
        }

        Arca::Stage EndStage()
        {
            Arca::Stage stage;
            stage.Add<FrameEndCurator>();
            return stage;
        }
    }

    namespace Logging
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Curator>();
        }
    }

    namespace Debug
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Statistics>();
        }
    }
}
