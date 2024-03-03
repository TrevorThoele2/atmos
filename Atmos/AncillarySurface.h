#pragma once

#include "Surface.h"

namespace Atmos::Render
{
    class AncillarySurface final : public Surface<AncillarySurface>
    {
    public:
        AncillarySurface(Init init, ResourcePtr&& resource);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::AncillarySurface>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Render::AncillarySurface";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::AncillarySurface, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Render::AncillarySurface, BinaryArchive>
    {};
}