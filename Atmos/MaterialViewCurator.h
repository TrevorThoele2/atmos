#pragma once

#include <Arca/Curator.h>
#include <Arca/CompositeShardBatch.h>

#include "MaterialViewCore.h"
#include "Bounds.h"

namespace Atmos::Render
{
    class MaterialViewCurator final : public Arca::Curator
    {
    protected:
        void PostConstructImplementation() override;
        void WorkImplementation(Stage& stage) override;
    private:
        Arca::Batch<Arca::All<MaterialViewCore, Arca::Either<Bounds>>> toRender;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MaterialViewCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::MaterialViewCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Render::MaterialViewCurator, BinaryArchive>
    {};
}