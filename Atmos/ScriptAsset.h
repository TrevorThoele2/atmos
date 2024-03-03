#pragma once

#include "FileAsset.h"
#include "ScriptAssetData.h"

class asIScriptModule;

namespace Atmos::Asset
{
    class ScriptAsset final : public FileAsset<ScriptAssetData, ScriptAsset>
    {
    public:
        using SymbolName = Atmos::Name;
    public:
        using FileAsset::FileAsset;

        ScriptAsset& operator=(ScriptAsset&& arg) noexcept;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Asset::ScriptAsset>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "ScriptAsset";
        static bool ShouldCreate(
            Reliquary& reliquary, const ::Atmos::Name& name, ::Atmos::Asset::ScriptAsset::DataPtr&& data);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::ScriptAsset, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Asset::ScriptAsset, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}