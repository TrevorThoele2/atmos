#include "TypeRegistration.h"

#include "InputCurator.h"
#include "InputInformation.h"

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

#include "PositionedSound.h"
#include "UniversalSound.h"
#include "AudioCurator.h"

#include "ActionAsset.h"
#include "AudioAsset.h"
#include "ImageAsset.h"
#include "ImageMaterialAsset.h"
#include "LineMaterialAsset.h"
#include "RegionMaterialAsset.h"
#include "ScriptAsset.h"
#include "ShaderAsset.h"
#include "MappedAssets.h"
#include "ActionAssetCurator.h"
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

#include "WorldCurator.h"

#include "DataCore.h"
#include "DataCurator.h"

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
        Asset::Resource::Manager& assetResourceManager,
        Logging::Logger& logger)
    {
        Asset::RegisterTypes(origin, assetResourceManager);
        Spatial::RegisterTypes(origin);
        Entity::RegisterTypes(origin);
        Frame::RegisterTypes(origin);
        Data::RegisterTypes(origin);
        Logging::RegisterTypes(origin, logger);
        Debug::RegisterTypes(origin);
    }

    void RegisterFieldTypes(
        Arca::ReliquaryOrigin& origin,
        Asset::Resource::Manager& assetResourceManager,
        Logging::Logger& logger)
    {
        Audio::RegisterTypes(origin);
        Input::RegisterTypes(origin);
        Render::RegisterTypes(origin);
        Scripting::RegisterTypes(origin);
        RegisterCommonTypes(origin, assetResourceManager, logger);
    }

    void RegisterFieldTypes(
        Arca::ReliquaryOrigin& origin,
        Asset::Resource::Manager& assetResourceManager,
        Audio::Manager& audio,
        Input::Manager& input,
        Render::GraphicsManager& graphics,
        Scripting::Manager& scripts,
        World::Manager& world,
        Spatial::ScreenSize screenSize,
        Window::WindowBase& window,
        Logging::Logger& logger)
    {
        Audio::RegisterTypes(origin, audio);
        Input::RegisterTypes(origin, input);
        Render::RegisterTypes(origin, graphics, screenSize, window.Handle());
        Scripting::RegisterTypes(origin, scripts);
        Window::RegisterTypes(origin, window);
        World::RegisterTypes(origin, world);
        RegisterCommonTypes(origin, assetResourceManager, logger);
    }

    void RegisterFieldStages(Arca::ReliquaryOrigin& origin)
    {
        auto pipeline = Arca::Pipeline();
        pipeline.push_back(Frame::StartStage());
        pipeline.push_back(Input::Stage());
        pipeline.push_back(Scripting::Stage());
        pipeline.push_back(Render::Stage());
        pipeline.push_back(Audio::Stage());
        pipeline.push_back(Frame::EndStage());
        origin.CuratorCommandPipeline<Work>(pipeline);
    }

    namespace Input
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Information>();
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
            origin
                .Register<SoundCore>()
                .Register<PositionedSound>()
                .Register<UniversalSound>();
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
        void RegisterTypes(Arca::ReliquaryOrigin& origin, Resource::Manager& resourceManager)
        {
            origin
                .Register<Core>()
                .Register<Action>()
                .Register<Audio>()
                .Register<Image>()
                .Register<ImageMaterial>()
                .Register<LineMaterial>()
                .Register<RegionMaterial>()
                .Register<Script>()
                .Register<Shader>()
                .Register<Mapped<Action>>()
                .Register<Mapped<Audio>>()
                .Register<Mapped<Image>>()
                .Register<Mapped<ImageMaterial>>()
                .Register<Mapped<LineMaterial>>()
                .Register<Mapped<RegionMaterial>>()
                .Register<Mapped<Script>>()
                .Register<Mapped<Shader>>()
                .Register<ActionCurator>()
                .Register<AudioCurator>(std::ref(resourceManager))
                .Register<ImageCurator>(std::ref(resourceManager))
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

    namespace World
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin, Manager& manager)
        {
            origin
                .Register<Curator>(std::ref(manager));
        }
    }

    namespace Data
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<DataCore>()
                .Register<DataCurator>();
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
