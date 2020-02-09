#pragma once

#include "FileAsset.h"
#include "ShaderAssetData.h"

namespace Atmos::Asset
{
    class ShaderAsset final : public FileAsset<ShaderAssetData, ShaderAsset>
    {
    public:
        explicit ShaderAsset(Init init);
        ShaderAsset(Init init, const Atmos::Name& name, DataPtr&& data);
        ShaderAsset(ShaderAsset&& arg) noexcept;

        ShaderAsset& operator=(ShaderAsset&& arg) noexcept;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Asset::ShaderAsset>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "ShaderAsset";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const ::Atmos::Name& name,
            ::Atmos::Asset::ShaderAsset::DataPtr&& data);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::ShaderAsset, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Asset::ShaderAsset, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}