#pragma once

#include "FileAsset.h"

#include "ObjectScribe.h"

class asIScriptModule;

namespace Atmos
{
    class ObjectManager;
}

namespace Atmos::Asset
{
    class ScriptAssetData;

    class ScriptAsset : public FileAsset
    {
    public:
        typedef Name SymbolName;
    public:
        typedef ScriptAssetData DataT;
        typedef std::unique_ptr<DataT> DataPtr;
    public:
        ScriptAsset(ObjectManager& manager, const File::Name& fileName, DataPtr&& data);
        ScriptAsset(const ScriptAsset& arg);
        ScriptAsset(const ::Inscription::BinaryTableData<ScriptAsset>& data);

        DataT* Data();
        const DataT* Data() const;
        template<class RealDataT>
        RealDataT* DataAs();
        template<class RealDataT>
        const RealDataT* DataAs() const;

        asIScriptModule* UnderlyingModule();
        const asIScriptModule* UnderlyingModule() const;

        ObjectTypeDescription TypeDescription() const override;
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
        ScriptAssetData(ObjectManager& objectManager);

        std::unique_ptr<ScriptAssetData> Clone() const;

        void Initialize(const Name& name, const File::Name& fileName);
    private:
        ObjectManager* objectManager;
    private:
        asIScriptModule* module;
        bool isInitialized;
    private:
        friend ScriptAsset;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Asset::ScriptAsset> : ObjectTraitsBase<Asset::ScriptAsset>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Asset::FileAsset> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Asset::ScriptAsset, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Asset::ScriptAsset, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Asset::ScriptAsset, BinaryArchive> :
        public ObjectScribe<::Atmos::Asset::ScriptAsset, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}