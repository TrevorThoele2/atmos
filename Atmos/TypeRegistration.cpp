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

#include "WindowCurator.h"

#include "FrameStartCurator.h"
#include "FrameEndCurator.h"
#include "FrameInformation.h"
#include "FrameSettings.h"

#include "LoggingCurator.h"

#include "DebugStatistics.h"

#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"

namespace Atmos
{
    void RegisterCommonTypes(Arca::ReliquaryOrigin& origin)
    {
        Asset::RegisterTypes(origin);
        Spatial::RegisterTypes(origin);
        Entity::RegisterTypes(origin);
        Script::RegisterTypes(origin);
        Frame::RegisterTypes(origin);
        Logging::RegisterTypes(origin);
        Debug::RegisterTypes(origin);
    }

    void RegisterFieldTypes(Arca::ReliquaryOrigin& origin)
    {
        Audio::RegisterTypes(origin);
        Input::RegisterTypes(origin);
        Render::RegisterTypes(origin);
        RegisterCommonTypes(origin);
    }

    void RegisterFieldTypes(
        Arca::ReliquaryOrigin& origin,
        Audio::AudioManager& audio,
        Input::Manager& input,
        Render::GraphicsManager& graphics,
        Spatial::ScreenSize screenSize,
        void* window)
    {
        Audio::RegisterTypes(origin, audio);
        Input::RegisterTypes(origin, input);
        Render::RegisterTypes(origin, graphics, screenSize, window);
        Window::RegisterTypes(origin);
        RegisterCommonTypes(origin);
    }

    void RegisterFieldStages(Arca::ReliquaryOrigin& origin)
    {
        auto pipeline = Arca::Pipeline();
        pipeline.push_back(Frame::StartStage());
        pipeline.push_back(Render::Stage());
        pipeline.push_back(Frame::EndStage());
        origin.CuratorPipeline(pipeline);
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

        }
    }

    namespace Window
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Curator>();
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

    namespace Script
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Instance>();
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
