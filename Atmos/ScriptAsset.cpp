
#include "ScriptAsset.h"

#include "ScriptEngineManager.h"
#include "AngelScriptAssert.h"
#include <angelscript.h>

#include "CurrentField.h"

#include <Inscription/TextFile.h>

namespace Atmos
{
    ScriptAsset::ScriptAsset(const FileName& fileName, DataPtr&& data) : nFileAsset(fileName), data(std::move(data))
    {
        data->Initialize(name, fileName);
    }

    ScriptAsset::ScriptAsset(const ScriptAsset& arg) : nFileAsset(arg), data((arg.data) ? arg.data->Clone() : nullptr)
    {}

    ScriptAsset::ScriptAsset(const ::Inscription::Table<ScriptAsset>& table) : INSCRIPTION_TABLE_GET_BASE(nFileAsset)
    {}

    ScriptAsset::DataT* ScriptAsset::Data()
    {
        return data.get();
    }

    const ScriptAsset::DataT* ScriptAsset::Data() const
    {
        return data.get();
    }

    asIScriptModule* ScriptAsset::UnderlyingModule()
    {
        return data->mod;
    }

    const asIScriptModule* ScriptAsset::UnderlyingModule() const
    {
        return data->mod;
    }

    ObjectTypeDescription ScriptAsset::TypeDescription() const
    {
        return ObjectTraits<ScriptAsset>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<ScriptAsset>::typeName = "ScriptAsset";

    ScriptAssetData::ScriptAssetData() : mod(nullptr), isInitialized(false)
    {}

    std::unique_ptr<ScriptAssetData> ScriptAssetData::Clone() const
    {
        return std::make_unique<ScriptAssetData>(*this);
    }

    void ScriptAssetData::Initialize(const Name& name, const FileName& fileName)
    {
        if (isInitialized)
            return;

        auto engine = GetLocalObjectManager()->FindSystem<Scripting::EngineManager>()->Engine();

        ::Inscription::TextInFile file(fileName);
        std::string fileAsString;
        file >> fileAsString;

        mod = engine->GetModule(fileName.GetWithoutExtension().c_str(), asGM_ALWAYS_CREATE);

        Scripting::AngelScriptAssert(mod->AddScriptSection(name.c_str(), fileAsString.c_str(), fileAsString.length()));

        mod->Build();

        isInitialized = true;
    }
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::ScriptAsset)
    {

    }
}