#pragma once

#include "Surface.h"

namespace Atmos::Render
{
    class MainSurface final : public Surface<MainSurface>
    {
    public:
        MainSurface(Init init, DataPtr&& data);
        MainSurface(MainSurface&& arg) noexcept = default;
    protected:
        void RenderStagedImpl(GraphicsManager& graphicsManager) override;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MainSurface>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "MainRenderSurface";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::MainSurface, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Render::MainSurface, BinaryArchive>
    {};
}