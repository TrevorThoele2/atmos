#pragma once

#include "Surface.h"

namespace Atmos::Render
{
    class MainSurface final : public Surface<MainSurface>
    {
    public:
        MainSurface(Init init);
        MainSurface(MainSurface&& arg) noexcept = default;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MainSurface>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Render::MainSurface";
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