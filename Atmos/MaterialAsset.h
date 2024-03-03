#pragma once

#include "Asset.h"

#include "ImageAsset.h"
#include "ShaderAsset.h"

namespace Atmos::Asset
{
    class MaterialAsset final : public Asset<MaterialAsset>
    {
    public:
        using GridDimension = int;
    public:
        explicit MaterialAsset(Init init);
        MaterialAsset(Init init, const Atmos::Name& name, GridDimension columns, GridDimension rows);
        MaterialAsset(MaterialAsset&& arg) noexcept;

        MaterialAsset& operator=(MaterialAsset&& arg) noexcept;

        [[nodiscard]] GridDimension Columns() const;
        [[nodiscard]] GridDimension Rows() const;

        [[nodiscard]] Arca::RelicIndex<ImageAsset> Image() const;
        [[nodiscard]] Arca::RelicIndex<ShaderAsset> Shader() const;
    private:
        GridDimension columns = 0;
        GridDimension rows = 0;

        Arca::RelicIndex<ImageAsset> image;
        Arca::RelicIndex<ShaderAsset> shader;
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
        static bool ShouldCreate(Reliquary& reliquary, const ::Atmos::Name& name);
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