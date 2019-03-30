#pragma once

#include "Asset.h"

#include "ObjectSerialization.h"

class asIScriptModule;

namespace Atmos
{
    class ScriptAssetData;

    class ScriptAsset : public nFileAsset
    {
    public:
        typedef Name SymbolName;
    public:
        typedef ScriptAssetData DataT;
        typedef std::unique_ptr<DataT> DataPtr;
    public:
        ScriptAsset(const FileName& fileName, DataPtr&& data);
        ScriptAsset(const ScriptAsset& arg);
        ScriptAsset(const ::Inscription::Table<ScriptAsset>& table);

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
    };

    class ScriptAssetData
    {
    public:
        ScriptAssetData();

        std::unique_ptr<ScriptAssetData> Clone() const;

        void Initialize(const Name& name, const FileName& fileName);
    private:
        asIScriptModule* mod;
        bool isInitialized;
    private:
        friend ScriptAsset;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::ScriptAsset)
    {
    public:
        static void AddMembers(TableT& table);
    };
}