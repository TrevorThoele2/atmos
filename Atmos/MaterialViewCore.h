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

        void Material(Arca::RelicIndex<Asset::MaterialAsset> to);
        Arca::RelicIndex<Asset::MaterialAsset> Material() const;
        void MaterialIndex(Index to);
        [[nodiscard]] Index MaterialIndex() const;
        [[nodiscard]] AxisAlignedBox2D MaterialSlice() const;

        void Color(Color to);
        [[nodiscard]] Render::Color Color() const;

        void PatchShader(Arca::RelicIndex<Asset::ShaderAsset> to);
        [[nodiscard]] Arca::RelicIndex<Asset::ShaderAsset> PatchShader() const;
    private:
        Arca::RelicIndex<Asset::MaterialAsset> material;
        Index materialIndex = 0;
        AxisAlignedBox2D materialSlice;

        Render::Color color;

        Arca::RelicIndex<Asset::ShaderAsset> patchShader;
    private:
        void CalculateMaterialSlice();
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