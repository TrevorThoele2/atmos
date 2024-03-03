#pragma once

#include "AssetWithResource.h"
#include "ShaderAssetResource.h"

namespace Atmos::Asset
{
    class Shader final : public AssetWithResource<Resource::Shader, Shader>
    {
    public:
        Shader(Init init, const Atmos::Name& name, ResourcePtr&& resource);
        Shader(Init init, Arca::Serialization serialization);
        Shader(Shader&& arg) noexcept;

        Shader& operator=(Shader&& arg) noexcept;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Asset::Shader>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Asset::Shader";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const ::Atmos::Name& name,
            const ::Atmos::Asset::Shader::ResourcePtr& data);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::Shader, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Asset::Shader, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}