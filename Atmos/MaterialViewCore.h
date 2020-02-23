#pragma once

#include <Arca/Shard.h>

#include "MaterialAsset.h"
#include "Color.h"
#include "ShaderAsset.h"
#include "AxisAlignedBox2D.h"

#include <Arca/Serialization.h>

namespace Atmos::Render
{
    class MaterialViewCore
    {
    public:
        using Index = int;
    public:
        Arca::Index<Asset::MaterialAsset> material;
        Index materialIndex = 0;
        AxisAlignedBox2D materialSlice;

        Render::Color color;

        Arca::Index<Asset::ShaderAsset> patchShader;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MaterialViewCore>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static inline const TypeName typeName = "MaterialViewCore";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::MaterialViewCore, BinaryArchive> final
        : public ArcaCompositeScribe<Atmos::Render::MaterialViewCore, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}