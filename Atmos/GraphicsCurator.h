#pragma once

#include <Arca/Curator.h>

#include "GraphicsManager.h"
#include "ReconstructGraphics.h"

namespace Atmos::Render
{
    class GraphicsCurator final : public Arca::Curator
    {
    public:
        explicit GraphicsCurator(Init init);
    public:
        void Handle(const ReconstructGraphics& command);
    private:
        Arca::Postulate<GraphicsManager*> manager;

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
        using HandledCommands = HandledCommands<Atmos::Render::ReconstructGraphics>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::GraphicsCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Render::GraphicsCurator, BinaryArchive>
    {};
}