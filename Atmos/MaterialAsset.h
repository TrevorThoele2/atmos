#pragma once

#include "Asset.h"

#include "ImageAsset.h"
#include "ShaderAsset.h"

namespace Atmos::Asset
{
    class MaterialAsset final : public Asset<MaterialAsset>
    {
    public:
        explicit MaterialAsset(Init init);
        MaterialAsset(
            Init init,
            const Atmos::Name& name,
            Arca::Index<ShaderAsset> vertexShader,
            Arca::Index<ShaderAsset> fragmentShader);
        MaterialAsset(MaterialAsset&& arg) noexcept;

        MaterialAsset& operator=(MaterialAsset&& arg) noexcept;

        [[nodiscard]] Arca::Index<ShaderAsset> VertexShader() const;
        [[nodiscard]] Arca::Index<ShaderAsset> FragmentShader() const;
    private:
        Arca::Index<ShaderAsset> vertexShader;
        Arca::Index<ShaderAsset> fragmentShader;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Asset::MaterialAsset>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "MaterialAsset";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const Atmos::Name& name,
            Index<Atmos::Asset::ShaderAsset> vertexShader,
            Index<Atmos::Asset::ShaderAsset> fragmentShader);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::MaterialAsset, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Asset::MaterialAsset, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}