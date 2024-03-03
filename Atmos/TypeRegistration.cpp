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
#include "SurfaceCurator.h"
#include "GraphicsSettings.h"

#include "AudioCurator.h"

#include "AudioAsset.h"
#include "ImageAsset.h"
#include "ImageMaterialAsset.h"
#include "LineMaterialAsset.h"
#include "RegionMaterialAsset.h"
#include "ScriptAsset.h"
#include "ShaderAsset.h"
#include "MappedAssets.h"
#include "AudioAssetCurator.h"
#include "ImageAssetCurator.h"
#include "ImageMaterialAssetCurator.h"
#include "LineMaterialAssetCurator.h"
#include "RegionMaterialAssetCurator.h"
#include "ScriptAssetCurator.h"
#include "ShaderAssetCurator.h"

#include "Entity.h"
#include "EntityCurator.h"
#include "EntityPrototype.h"
#include "MappedEntities.h"
#include "CurrentActualizingEntity.h"

#include "WindowCurator.h"
#include "WindowInformation.h"

#include "Script.h"
#include "ScriptCurator.h"
#include "CurrentExecutingScript.h"

#include "FrameStartCurator.h"
#include "FrameEndCurator.h"
#include "FrameInformation.h"
#include "FrameSettings.h"

#include "LoggingCurator.h"
#include "LoggingInformation.h"

#include "DebugStatistics.h"

#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"

#include "Work.h"

#include <Arca/LocalRelic.h>

namespace Atmos
{
    void RegisterArcaTypes(Arca::ReliquaryOrigin& origin)
    {
        origin
            .Register<Arca::OpenRelic>()
            .Register<Arca::ClosedRelic>();
    }

    void RegisterCommonTypes(
        Arca::ReliquaryOrigin& origin,
        Asset::ImageManager& imageAssetManager,
        Logging::Logger& logger)
    {
        Asset::RegisterTypes(origin, imageAssetManager);
        Spatial::RegisterTypes(origin);
        Entity::RegisterTypes(origin);
        Frame::RegisterTypes(origin);
        Logging::RegisterTypes(origin, logger);
        Debug::RegisterTypes(origin);
    }

    void RegisterFieldTypes(
        Arca::ReliquaryOrigin& origin,
        Asset::ImageManager& imageAssetManager,
        Logging::Logger& logger)
    {
        Audio::RegisterTypes(origin);
        Input::RegisterTypes(origin);
        Render::RegisterTypes(origin);
        Scripting::RegisterTypes(origin);
        RegisterCommonTypes(origin, imageAssetManager, logger);
    }

    void RegisterFieldTypes(
        Arca::ReliquaryOrigin& origin,
        Asset::ImageManager& imageAssetManager,
        Audio::AudioManager& audio,
        Input::Manager& input,
        Render::GraphicsManager& graphics,
        Scripting::Manager& scripts,
        Spatial::ScreenSize screenSize,
        Window::WindowBase& window,
        Logging::Logger& logger)
    {
        Audio::RegisterTypes(origin, audio);
        Input::RegisterTypes(origin, input);
        Render::RegisterTypes(origin, graphics, screenSize, window.Handle());
        Scripting::RegisterTypes(origin, scripts);
        Window::RegisterTypes(origin, window);
        RegisterCommonTypes(origin, imageAssetManager, logger);
    }

    void RegisterFieldStages(Arca::ReliquaryOrigin& origin)
    {
        auto pipeline = Arca::Pipeline();
        pipeline.push_back(Frame::StartStage());
        pipeline.push_back(Scripting::Stage());
        pipeline.push_back(Render::Stage());
        pipeline.push_back(Frame::EndStage());
        origin.CuratorCommandPipeline<Work>(pipeline);
    }

    namespace Input
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {

        }

        void RegisterTypes(Arca::ReliquaryOrigin& origin, Manager& manager)
        {

        }
    }

    namespace Render
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
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
                .Register<CameraCurator>()
                .Register<Curator>()
                .Register<SurfaceCore>()
                .Register<SurfaceCurator>()
                .Register<GraphicsSettings>();
        }

        void RegisterTypes(
            Arca::ReliquaryOrigin& origin, GraphicsManager& manager, Spatial::ScreenSize screenSize, void* window)
        {
            RegisterTypes(origin);

            origin
                .Register<GraphicsCurator>(std::ref(manager))
                .Register<Camera>(screenSize)
                .Register<MainSurface>(std::ref(manager), window);
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
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {

        }

        void RegisterTypes(Arca::ReliquaryOrigin& origin, AudioManager& manager)
        {
            origin
                .Register<AudioCurator>(std::ref(manager));
        }
    }

    namespace Window
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin, WindowBase& window)
        {
            origin
                .Register<Curator>()
                .Register<Information>(std::ref(window));
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
        void RegisterTypes(Arca::ReliquaryOrigin& origin, ImageManager& manager)
        {
            origin
                .Register<Core>()
                .Register<Audio>()
                .Register<Image>()
                .Register<ImageMaterial>()
                .Register<LineMaterial>()
                .Register<RegionMaterial>()
                .Register<Script>()
                .Register<Shader>()
                .Register<Mapped<Audio>>()
                .Register<Mapped<Image>>()
                .Register<Mapped<ImageMaterial>>()
                .Register<Mapped<LineMaterial>>()
                .Register<Mapped<RegionMaterial>>()
                .Register<Mapped<Script>>()
                .Register<Mapped<Shader>>()
                .Register<AudioCurator>()
                .Register<ImageCurator>(std::ref(manager))
                .Register<ImageMaterialCurator>()
                .Register<LineMaterialCurator>()
                .Register<RegionMaterialCurator>()
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
                .Register<Mapped>()
                .Register<CurrentActualizing>();
        }
    }

    namespace Scripting
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Script>()
                .Register<CurrentExecutingScript>();
        }

        void RegisterTypes(Arca::ReliquaryOrigin& origin, Manager& manager)
        {
            RegisterTypes(origin);

            origin
                .Register<Curator>(std::ref(manager));
        }

        Arca::Stage Stage()
        {
            Arca::Stage stage;
            stage.Add<Curator>();
            return stage;
        }
    }

    namespace Frame
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Information>()
                .Register<StartCurator>()
                .Register<EndCurator>()
                .Register<Settings>();
        }

        Arca::Stage StartStage()
        {
            Arca::Stage stage;
            stage.Add<StartCurator>();
            return stage;
        }

        Arca::Stage EndStage()
        {
            Arca::Stage stage;
            stage.Add<EndCurator>();
            return stage;
        }
    }

    namespace Logging
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin, Logger& logger)
        {
            origin
                .Register<Curator>()
                .Register<Information>(std::ref(logger));
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
