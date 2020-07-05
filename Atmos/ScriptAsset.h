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
        Script(Init init, const ::Atmos::Name& name, ResourcePtr&& resource);
        Script(Init init, Arca::Serialization serialization);
        Script(Script&& arg) noexcept;

        Script& operator=(Script&& arg) noexcept;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Asset::Script>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Asset::Script";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const ::Atmos::Name& name,
            const ::Atmos::Asset::Script::ResourcePtr& data);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::Script, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Asset::Script, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}