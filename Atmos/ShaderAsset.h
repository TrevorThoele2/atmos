#pragma once

#include "AssetWithResource.h"
#include "ShaderAssetResource.h"

#include "LoadAssetsUserContext.h"
#include "CreateShaderAssetResource.h"

namespace Atmos::Asset
{
    class Shader final : public AssetWithResource<Resource::Shader>
    {
    public:
        Shader(Arca::RelicInit init, const Atmos::Name& name, ResourcePtr&& resource);
        Shader(Arca::RelicInit init, Arca::Serialization serialization);
        Shader(Shader&& arg) noexcept;

        Shader& operator=(Shader&& arg) noexcept;
    public:
        void Setup(ResourcePtr&& set);
    private:
        Arca::RelicInit init;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Shader>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Asset::Shader"; }
        static bool ShouldCreate(
            Reliquary& reliquary,
            const Atmos::Name& name,
            const Atmos::Asset::Shader::ResourcePtr& data);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::Shader> final
    {
    public:
        using ObjectT = Atmos::Asset::Shader;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            BaseScriven<Atmos::Asset::AssetWithResource<Atmos::Asset::Resource::Shader>>(
                object, archive);

            if (archive.IsInput())
            {
                auto& assetUserContext = *archive.template UserContext<LoadAssetsUserContext>();

                auto extracted = assetUserContext.LoadShaderData(object.Name());
                if (extracted)
                {
                    using CreateResource = Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Shader>;
                    auto resource = object.init.owner.Do(CreateResource
                    {
                        extracted->memory,
                        object.Name()
                    });

                    object.Setup(std::move(resource));
                }
            }
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::Shader, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Shader>;
    };
}