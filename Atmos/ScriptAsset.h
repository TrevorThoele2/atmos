#pragma once

#include "FileAsset.h"
#include <Arca/ClosedTypedRelicAutomation.h>

class asIScriptModule;

namespace Atmos::Script::Angel
{
    class ScriptCurator;
}

namespace Atmos::Asset
{
    class ScriptAssetData;

    class ScriptAsset final : public Arca::ClosedTypedRelicAutomation<ScriptAsset>, public FileAsset
    {
    public:
        using SymbolName = Atmos::Name;
    public:
        using DataT = ScriptAssetData;
        using DataPtr = std::unique_ptr<DataT>;
    public:
        ScriptAsset();
        ScriptAsset(const ScriptAsset& arg) = delete;
        ScriptAsset(ScriptAsset&& arg) noexcept = default;
        explicit ScriptAsset(const ::Inscription::BinaryTableData<ScriptAsset>& data);

        [[nodiscard]] DataT* Data();
        [[nodiscard]] const DataT* Data() const;
        template<class RealDataT>
        [[nodiscard]] RealDataT* DataAs();
        template<class RealDataT>
        [[nodiscard]] const RealDataT* DataAs() const;

        [[nodiscard]] asIScriptModule* UnderlyingModule();
        [[nodiscard]] const asIScriptModule* UnderlyingModule() const;
    public:
        void Initialize(const File::Name& fileName, DataPtr&& data);
    private:
        DataPtr data;
    };

    template<class RealDataT>
    RealDataT* ScriptAsset::DataAs()
    {
        return static_cast<RealDataT*>(data.get());
    }

    template<class RealDataT>
    const RealDataT* ScriptAsset::DataAs() const
    {
        return static_cast<RealDataT*>(data.get());
    }

    class ScriptAssetData
    {
    public:
        explicit ScriptAssetData(Script::Angel::ScriptCurator& scriptCurator);
        [[nodiscard]] std::unique_ptr<ScriptAssetData> Clone() const;
        void Initialize(const Name& name, const File::Name& fileName);
    private:
        Script::Angel::ScriptCurator* scriptCurator;

        asIScriptModule* module = nullptr;
        bool isInitialized = false;
    private:
        friend ScriptAsset;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Asset::ScriptAsset>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
        static bool ShouldCreate(
            Reliquary& reliquary, const ::Atmos::File::Name& fileName, ::Atmos::Asset::ScriptAsset::DataPtr&& data);
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Asset::ScriptAsset, BinaryArchive> :
        TableDataBase<::Atmos::Asset::ScriptAsset, BinaryArchive>
    {
        Base<::Atmos::Asset::FileAsset> base;
    };

    template<>
    class Scribe<::Atmos::Asset::ScriptAsset, BinaryArchive> final :
        public ArcaTableScribe<::Atmos::Asset::ScriptAsset, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();
        };
    };
}