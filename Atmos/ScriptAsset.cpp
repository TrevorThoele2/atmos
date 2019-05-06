
#include "ScriptAsset.h"

#include "ObjectManager.h"
#include "AngelScriptSystem.h"

#include "AngelScriptAssert.h"
#include <angelscript.h>

#include <Inscription/InputTextFile.h>

namespace Atmos
{
    ScriptAsset::ScriptAsset(ObjectManager& manager, const FileName& fileName, DataPtr&& data) :
        FileAsset(manager, fileName), data(std::move(data))
    {
        data->Initialize(name, fileName);
    }

    ScriptAsset::ScriptAsset(const ScriptAsset& arg) : FileAsset(arg), data((arg.data) ? arg.data->Clone() : nullptr)
    {}

    ScriptAsset::ScriptAsset(const ::Inscription::Table<ScriptAsset>& table) : INSCRIPTION_TABLE_GET_BASE(FileAsset)
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
        return data->module;
    }

    const asIScriptModule* ScriptAsset::UnderlyingModule() const
    {
        return data->module;
    }

    ObjectTypeDescription ScriptAsset::TypeDescription() const
    {
        return ObjectTraits<ScriptAsset>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<ScriptAsset>::typeName = "ScriptAsset";

    ScriptAssetData::ScriptAssetData(ObjectManager& objectManager) : objectManager(&objectManager), module(nullptr), isInitialized(false)
    {}

    std::unique_ptr<ScriptAssetData> ScriptAssetData::Clone() const
    {
        return std::make_unique<ScriptAssetData>(*this);
    }

    void ScriptAssetData::Initialize(const Name& name, const FileName& fileName)
    {
        if (isInitialized)
            return;

        auto engine = objectManager->FindSystem<Scripting::System>()->Engine();

        ::Inscription::InputTextFile file(fileName);
        String fileAsString;
        file >> fileAsString;

        module = engine->GetModule(fileName.GetWithoutExtension().c_str(), asGM_ALWAYS_CREATE);

        Scripting::AngelScriptAssert(module->AddScriptSection(name.c_str(), fileAsString.c_str(), fileAsString.length()));

        module->Build();

        isInitialized = true;
    }
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::ScriptAsset)
    {

    }
}