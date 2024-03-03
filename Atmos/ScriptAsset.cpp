#include "ScriptAsset.h"

#include "ObjectManager.h"
#include "AngelScriptSystem.h"

#include "AngelScriptAssert.h"
#include <angelscript.h>

#include <Inscription/InputTextFile.h>

namespace Atmos::Asset
{
    ScriptAsset::ScriptAsset(ObjectManager& manager, const File::Name& fileName, DataPtr&& data) :
        FileAsset(manager, fileName), data(std::move(data))
    {
        data->Initialize(name, fileName);
    }

    ScriptAsset::ScriptAsset(const ScriptAsset& arg) :
        FileAsset(arg), data((arg.data) ? arg.data->Clone() : nullptr)
    {}

    ScriptAsset::ScriptAsset(const ::Inscription::BinaryTableData<ScriptAsset>& data) :
        FileAsset(std::get<0>(data.bases))
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

    ScriptAssetData::ScriptAssetData(ObjectManager& objectManager) :
        objectManager(&objectManager), module(nullptr), isInitialized(false)
    {}

    std::unique_ptr<ScriptAssetData> ScriptAssetData::Clone() const
    {
        return std::make_unique<ScriptAssetData>(*this);
    }

    void ScriptAssetData::Initialize(const Name& name, const File::Name& fileName)
    {
        if (isInitialized)
            return;

        auto engine = objectManager->FindSystem<Script::ScriptSystem>()->Engine();

        ::Inscription::InputTextFile file(fileName.GetValue());
        String fileAsString;
        file.ReadData(fileAsString);

        module = engine->GetModule(fileName.GetWithoutExtension().c_str(), asGM_ALWAYS_CREATE);

        Script::AngelScriptAssert(
            module->AddScriptSection(name.c_str(),
                fileAsString.c_str(),
                fileAsString.length()));

        module->Build();

        isInitialized = true;
    }
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Asset::ScriptAsset>::typeName = "ScriptAsset";
}