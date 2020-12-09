#pragma once

#include <Arca/Curator.h>

#include "GraphicsManager.h"

#include "InitializeGraphics.h"
#include "ReconstructGraphics.h"
#include "CreateSurfaceResource.h"
#include "SetFullscreen.h"
#include "ChangeVerticalSync.h"
#include "PruneGraphicsResources.h"
#include "CompileShader.h"

#include "CreateImageAssetResource.h"
#include "CreateShaderAssetResource.h"

namespace Atmos::Render
{
    class GraphicsCurator final : public Arca::Curator
    {
    public:
        explicit GraphicsCurator(Init init, GraphicsManager& manager);
    public:
        void Handle(const InitializeGraphics& command);
        void Handle(const ReconstructGraphics& command);
        std::unique_ptr<Resource::Surface> Handle(const Resource::CreateSurface& command);
        void Handle(const SetFullscreen& command);
        void Handle(const ChangeVerticalSync& command);
        void Handle(const PruneGraphicsResources& command);
        File::Path Handle(const CompileShader& command);

        std::unique_ptr<Asset::Resource::Image> Handle(const Asset::Resource::Create<Asset::Resource::Image>& command);
        std::unique_ptr<Asset::Resource::Shader> Handle(const Asset::Resource::Create<Asset::Resource::Shader>& command);
    private:
        GraphicsManager* manager;

        void AttemptReconstruct(const Spatial::ScreenSize& size);
    private:
        template<class T>
        std::vector<T*> MutablePointersOf();
    };

    template<class T>
    std::vector<T*> GraphicsCurator::MutablePointersOf()
    {
        auto batch = Owner().Batch<T>();
        std::vector<T*> returnValue;
        returnValue.reserve(batch.Size());
        for (auto object = batch.begin(); object != batch.end(); ++object)
            returnValue.push_back(MutablePointer().Of<T>(object.ID()));
        return returnValue;
    }
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::GraphicsCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Render::GraphicsCurator";
        using HandledCommands = HandledCommands<
            Atmos::Render::InitializeGraphics,
            Atmos::Render::ReconstructGraphics,
            Atmos::Render::Resource::CreateSurface,
            Atmos::Render::SetFullscreen,
            Atmos::Render::ChangeVerticalSync,
            Atmos::Render::PruneGraphicsResources,
            Atmos::Render::CompileShader,
            Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>,
            Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Shader>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Render::GraphicsCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::GraphicsCurator>;
    };
}