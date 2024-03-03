#pragma once

#include <Arca/Curator.h>

#include "GraphicsManager.h"

#include "InitializeGraphics.h"
#include "ReconstructGraphics.h"
#include "SetupMainSurfaceData.h"
#include "CreateSurfaceData.h"
#include "SetFullscreen.h"
#include "ChangeVerticalSync.h"

#include "CreateImageAssetData.h"
#include "CreateShaderAssetData.h"

namespace Atmos::Render
{
    class GraphicsCurator final : public Arca::Curator
    {
    public:
        explicit GraphicsCurator(Init init, GraphicsManager& manager);
    public:
        void Handle(const InitializeGraphics& command);
        void Handle(const ReconstructGraphics& command);
        void Handle(const SetupMainSurfaceData& command);
        std::unique_ptr<SurfaceData> Handle(const CreateSurfaceData& command);
        void Handle(const SetFullscreen& command);
        void Handle(const ChangeVerticalSync& command);

        std::unique_ptr<Asset::ImageData> Handle(const Asset::CreateData<Asset::ImageData>& command);
        std::unique_ptr<Asset::ShaderData> Handle(const Asset::CreateData<Asset::ShaderData>& command);
    private:
        GraphicsManager* manager;

        template<class T>
        std::vector<T*> MutablePointersOf();
    };

    template<class T>
    std::vector<T*> GraphicsCurator::MutablePointersOf()
    {
        auto batch = Owner().Batch<T>();
        std::vector<T*> returnValue;
        returnValue.reserve(batch.Size());
        for (auto& loop : batch)
            returnValue.push_back(MutablePointer().Of(loop));
        return returnValue;
    }
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::GraphicsCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "GraphicsCurator";
        using HandledCommands = HandledCommands<
            Atmos::Render::InitializeGraphics,
            Atmos::Render::ReconstructGraphics,
            Atmos::Render::SetupMainSurfaceData,
            Atmos::Render::CreateSurfaceData,
            Atmos::Render::SetFullscreen,
            Atmos::Render::ChangeVerticalSync,
            Atmos::Asset::CreateData<Atmos::Asset::ImageData>,
            Atmos::Asset::CreateData<Atmos::Asset::ShaderData>>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::GraphicsCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Render::GraphicsCurator, BinaryArchive>
    {};
}