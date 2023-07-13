#include "TypeRegistration.h"

#include "InputCurator.h"
#include "InputInformation.h"

#include "Bounds.h"
#include "BoundsCurator.h"

#include "FileCurator.h"

#include "RenderCore.h"
#include "StaticImage.h"
#include "DynamicImage.h"
#include "ImageCurator.h"
#include "LineCurator.h"
#include "GridRegion.h"
#include "TextCore.h"
#include "DynamicText.h"
#include "GridRegionCurator.h"
#include "RenderCurator.h"
#include "GraphicsCurator.h"
#include "Camera.h"
#include "SurfaceCurator.h"
#include "TextCurator.h"
#include "GraphicsSettings.h"
#include "ViewSlice.h"
#include "RenderStartCurator.h"
#include "RenderEndCurator.h"
#include "StagedRasters.h"
#include "ExecutingMaterial.h"

#include "UIImage.h"
#include "UIText.h"

#include "PositionedSound.h"
#include "UniversalSound.h"
#include "AudioCurator.h"

#include "ActionAsset.h"
#include "AudioAsset.h"
#include "ImageAsset.h"
#include "MaterialAsset.h"
#include "ScriptAsset.h"
#include "FontAsset.h"
#include "ShaderAsset.h"
#include "MappedAssets.h"
#include "ActionAssetCurator.h"
#include "AudioAssetCurator.h"
#include "ImageAssetCurator.h"
#include "MaterialAssetCurator.h"
#include "ScriptAssetCurator.h"
#include "FontAssetCurator.h"
#include "ShaderAssetCurator.h"

#include "Entity.h"
#include "EntityCurator.h"
#include "EntityPrototype.h"
#include "MappedEntities.h"
#include "CurrentActualizingEntity.h"
#include "EntityActualizationCurator.h"

#include "WindowCurator.h"
#include "WindowInformation.h"

#include "Script.h"
#include "ScriptCurator.h"

#include "FrameStartCurator.h"
#include "FrameEndCurator.h"
#include "FrameIdleCurator.h"
#include "FrameInformation.h"
#include "FrameSettings.h"

#include "Map.h"
#include "WorldCurator.h"

#include "DataCore.h"
#include "DataCurator.h"

#include "LoggingCurator.h"
#include "LoggingInformation.h"

#include "DiagnosticsStatistics.h"
#include "DiagnosticsCurator.h"

#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"

#include "Work.h"

#include <Arca/OpenRelic.h>

#include "RandomInformation.h"

namespace Atmos
{
    void RegisterArcaTypes(Arca::ReliquaryOrigin& origin)
    {
        origin
            .Register<Arca::OpenRelic>();
    }

    void RegisterCommonTypes(
        Arca::ReliquaryOrigin& origin,
        Asset::Resource::Manager& assetResourceManager,
        Logging::Logger& logger)
    {
        Asset::RegisterTypes(origin, assetResourceManager);
        Spatial::RegisterTypes(origin);
        UI::RegisterTypes(origin);
        Entity::RegisterTypes(origin);
        Frame::RegisterTypes(origin);
        Data::RegisterTypes(origin);
        Logging::RegisterTypes(origin, logger);
        Diagnostics::RegisterTypes(origin);
        Random::RegisterTypes(origin);
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
        Render::TextManager& text,
        Scripting::Manager& scripts,
        World::Manager& world,
        Spatial::Size2D screenSize,
        Window::WindowBase& window,
        Logging::Logger& logger)
    {
        Audio::RegisterTypes(origin, audio);
        Input::RegisterTypes(origin, input);
        Render::RegisterTypes(origin, graphics, text, screenSize, window.Handle());
        Scripting::RegisterTypes(origin, scripts);
        Window::RegisterTypes(origin, window);
        World::RegisterTypes(origin, world);
        RegisterCommonTypes(origin, assetResourceManager, logger);
    }

    void RegisterFieldStages(Arca::ReliquaryOrigin& origin)
    {
        auto pipeline = Arca::Pipeline();
        pipeline.push_back(Frame::StartStage());
        pipeline.push_back(Entity::ActualizationStage());
        pipeline.push_back(Input::Stage());
        pipeline.push_back(Scripting::Stage());
        pipeline.push_back(Render::Stage());
        pipeline.push_back(Audio::Stage());
        pipeline.push_back(World::Stage());
        pipeline.push_back(Diagnostics::Stage());
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
                .Register<StartCurator>()
                .Register<RenderCore>()
                .Register<ImageCore>()
                .Register<StaticImage>()
                .Register<DynamicImage>()
                .Register<Line>()
                .Register<GridRegion>()
                .Register<TextCore>()
                .Register<DynamicText>()
                .Register<SurfaceCore>()
                .Register<SurfaceCurator>()
                .Register<GraphicsSettings>()
                .Register<ViewSlice>()
                .Register<Raster::Staged>()
                .Register<Raster::ExecutingMaterial>()
                .Register<EndCurator>();
        }

        void RegisterTypes(
            Arca::ReliquaryOrigin& origin,
            GraphicsManager& graphicsManager,
            TextManager& textManager,
            Spatial::Size2D screenSize,
            void* window)
        {
            RegisterTypes(origin);

            origin
                .Register<GraphicsCurator>(std::ref(graphicsManager))
                .Register<ImageCurator>()
                .Register<LineCurator>()
                .Register<GridRegionCurator>()
                .Register<TextCurator>(std::ref(textManager))
                .Register<Curator>(std::ref(graphicsManager))
                .Register<Camera>(screenSize)
                .Register<MainSurface>(std::ref(graphicsManager), window);
        }

        Arca::Stage Stage()
        {
            Arca::Stage stage;
            stage.Add<StartCurator>();
            stage.Add<ImageCurator>();
            stage.Add<LineCurator>();
            stage.Add<GridRegionCurator>();
            stage.Add<TextCurator>();
            stage.Add<Curator>();
            stage.Add<EndCurator>();
            return stage;
        }
    }

    namespace UI
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Image>()
                .Register<Text>();
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
                .Register<BoundsCurator>();
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
                .Register<Material>()
                .Register<Script>()
                .Register<Font>()
                .Register<Shader>()
                .Register<Mapped<Action>>()
                .Register<Mapped<Audio>>()
                .Register<Mapped<Image>>()
                .Register<Mapped<Material>>()
                .Register<Mapped<Script>>()
                .Register<Mapped<Font>>()
                .Register<Mapped<Shader>>()
                .Register<ActionCurator>()
                .Register<AudioCurator>(std::ref(resourceManager))
                .Register<ImageCurator>(std::ref(resourceManager))
                .Register<MaterialCurator>()
                .Register<ScriptCurator>()
                .Register<FontCurator>()
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
                .Register<CurrentActualizing>()
                .Register<ActualizationCurator>();
        }

        Arca::Stage ActualizationStage()
        {
            Arca::Stage stage;
            stage.Add<ActualizationCurator>();
            return stage;
        }
    }

    namespace Scripting
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Script>();
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
                .Register<IdleCurator>()
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
            stage.Add<IdleCurator>();
            return stage;
        }
    }

    namespace World
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin, Manager& manager)
        {
            origin
                .Register<Map>()
                .Register<Curator>(std::ref(manager));
        }

        Arca::Stage Stage()
        {
            Arca::Stage stage;
            stage.Add<Curator>();
            return stage;
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

    namespace Diagnostics
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Statistics>()
                .Register<Curator>();
        }

        Arca::Stage Stage()
        {
            Arca::Stage stage;
            stage.Add<Curator>();
            return stage;
        }
    }
    
    namespace Random
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin)
        {
            origin
                .Register<Information>();
        }
    }
}