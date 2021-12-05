#pragma once

#include <Arca/Curator.h>

#include "Work.h"
#include "ChangeColor.h"
#include "ChangeMaterialAsset.h"
#include "ChangeViewSlice.h"

namespace Atmos::Render
{
    class GraphicsManager;

    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init, GraphicsManager& graphicsManager);

        void Handle(const Work& command);
        void Handle(const ChangeColor& command);
        void Handle(const ChangeMaterialAsset& command);
        void Handle(const ChangeViewSlice& command);
    private:
        GraphicsManager* graphicsManager;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::Curator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Render::Curator";
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::Render::ChangeColor,
            Atmos::Render::ChangeMaterialAsset,
            Atmos::Render::ChangeViewSlice>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Render::Curator, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::Curator>;
    };
}