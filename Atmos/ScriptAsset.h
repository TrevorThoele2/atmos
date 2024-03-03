#pragma once

#include "Asset.h"

#include "ObjectScribe.h"

class asIScriptModule;

namespace Atmos
{
    class ScriptAssetData;
    class ObjectManager;

    class ScriptAsset : public FileAsset
    {
    public:
        typedef Name SymbolName;
    public:
        typedef ScriptAssetData DataT;
        typedef std::unique_ptr<DataT> DataPtr;
    public:
        ScriptAsset(ObjectManager& manager, const FileName& fileName, DataPtr&& data);
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

    template<>
    struct ObjectTraits<ScriptAsset> : ObjectTraitsBase<ScriptAsset>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<FileAsset> bases = {};
    };

    class ScriptAssetData
    {
    public:
        ScriptAssetData(ObjectManager& objectManager);

        std::unique_ptr<ScriptAssetData> Clone() const;

        void Initialize(const Name& name, const FileName& fileName);
    private:
        ObjectManager* objectManager;
    private:
        asIScriptModule* module;
        bool isInitialized;
    private:
        friend ScriptAsset;
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::ScriptAsset, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::ScriptAsset, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::ScriptAsset, BinaryArchive> : public ObjectScribe<::Atmos::ScriptAsset, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}