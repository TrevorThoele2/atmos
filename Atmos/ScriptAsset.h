#pragma once

#include "AssetWithResource.h"
#include "ScriptAssetResource.h"

class asIScriptModule;

namespace Atmos::Asset
{
    class Script final : public AssetWithResource<Resource::Script, Script>
    {
    public:
        using SymbolName = Atmos::Name;
    public:
        Script(Init init, const Atmos::Name& name, ResourcePtr&& resource);
        Script(Init init, Arca::Serialization serialization);
        Script(Script&& arg) noexcept;

        Script& operator=(Script&& arg) noexcept;
    public:
        void Setup(ResourcePtr&& set);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Script>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Asset::Script";
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
            BaseScriven<Atmos::Asset::AssetWithResource<Atmos::Asset::Resource::Script, Atmos::Asset::Script>>(
                object, archive);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::Script, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Script>;
    };
}