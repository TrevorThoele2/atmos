#pragma once

#include "AssetWithResource.h"
#include "ShaderAssetResource.h"

namespace Atmos::Asset
{
    class Shader final : public AssetWithResource<Resource::Shader, Shader>
    {
    public:
        explicit Shader(Init init);
        Shader(Init init, const Atmos::Name& name, ResourcePtr&& resource, const String& entryPoint);
        Shader(Shader&& arg) noexcept;

        Shader& operator=(Shader&& arg) noexcept;

        [[nodiscard]] const String& EntryPoint() const;
    private:
        String entryPoint;
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
        static inline const TypeName typeName = "ShaderAsset";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const ::Atmos::Name& name,
            const ::Atmos::Asset::Shader::ResourcePtr& data,
            const ::Atmos::String& entryPoint);
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