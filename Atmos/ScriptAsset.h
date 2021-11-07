#pragma once

#include "AssetWithResource.h"
#include "ScriptAssetResource.h"

#include "LoadAssetsUserContext.h"
#include "CreateScriptAssetResource.h"

namespace Atmos::Asset
{
    class Script final : public AssetWithResource<Resource::Script>
    {
    public:
        Script(Arca::RelicInit init, const Atmos::Name& name, ResourcePtr&& resource);
        Script(Arca::RelicInit init, Arca::Serialization serialization);
        Script(Script&& arg) noexcept;

        Script& operator=(Script&& arg) noexcept;

        bool operator==(const Script& arg) const;
        bool operator!=(const Script& arg) const;
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
    struct Traits<Atmos::Asset::Script>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Asset::Script";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const Atmos::Name& name,
            const Atmos::Asset::Script::ResourcePtr& data);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::Script> final
    {
    public:
        using ObjectT = Atmos::Asset::Script;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            BaseScriven<Atmos::Asset::AssetWithResource<Atmos::Asset::Resource::Script>>(
                object, archive);

            if (archive.IsInput())
            {
                auto& assetUserContext = *archive.template UserContext<LoadAssetsUserContext>();

                auto extracted = assetUserContext.LoadScriptData(object.Name());
                if (extracted)
                {
                    using CreateResource = Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Script>;
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
    struct ScribeTraits<Atmos::Asset::Script, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Script>;
    };
}