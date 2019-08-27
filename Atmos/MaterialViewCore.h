#pragma once

#include <Arca/ShardTraits.h>

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

        void Material(Arca::Ptr<Asset::MaterialAsset> to);
        Arca::Ptr<Asset::MaterialAsset> Material() const;
        void MaterialIndex(Index to);
        [[nodiscard]] Index MaterialIndex() const;
        [[nodiscard]] AxisAlignedBox2D MaterialSlice() const;

        void Color(Color to);
        [[nodiscard]] Render::Color Color() const;

        void PatchShader(Arca::Ptr<Asset::ShaderAsset> to);
        [[nodiscard]] Arca::Ptr<Asset::ShaderAsset> PatchShader() const;
    private:
        Arca::Ptr<Asset::MaterialAsset> material;
        Index materialIndex = 0;
        AxisAlignedBox2D materialSlice;

        Render::Color color;

        Arca::Ptr<Asset::ShaderAsset> patchShader;
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
        static const TypeName typeName;
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