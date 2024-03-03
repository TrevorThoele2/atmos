#include "ScriptAsset.h"

#include "ShouldCreateAsset.h"

#include "AngelScriptCurator.h"

#include "AngelScriptResultVerification.h"
#include <angelscript.h>

#include <Inscription/InputTextFile.h>

namespace Atmos::Asset
{
    ScriptAsset::ScriptAsset() = default;

    ScriptAsset::ScriptAsset(const ::Inscription::BinaryTableData<ScriptAsset>& data) :
        FileAsset(data.base)
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

    void ScriptAsset::Initialize(const File::Name& fileName, DataPtr&& data)
    {
        SetFileName(fileName);
        this->data = std::move(data);
    }

    ScriptAssetData::ScriptAssetData(Script::Angel::ScriptCurator& scriptCurator) :
        scriptCurator(&scriptCurator)
    {}

    std::unique_ptr<ScriptAssetData> ScriptAssetData::Clone() const
    {
        return std::make_unique<ScriptAssetData>(*this);
    }

    void ScriptAssetData::Initialize(const Name& name, const File::Name& fileName)
    {
        if (isInitialized)
            return;

        auto engine = scriptCurator->Engine();

        ::Inscription::InputTextFile file(fileName.GetValue());
        String fileAsString;
        file.ReadData(fileAsString);

        module = engine->GetModule(fileName.GetWithoutExtension().c_str(), asGM_ALWAYS_CREATE);

        Script::Angel::VerifyResult(module->AddScriptSection
        (
            name.c_str(),
            fileAsString.c_str(),
            fileAsString.length()
        ));

        module->Build();

        isInitialized = true;
    }
}

namespace Arca
{
    const TypeName Traits<::Atmos::Asset::ScriptAsset>::typeName = "ScriptAsset";
    bool Traits<::Atmos::Asset::ScriptAsset>::ShouldCreate(
        Reliquary& reliquary, const ::Atmos::File::Name& fileName, ::Atmos::Asset::ScriptAsset::DataPtr&& data)
    {
        return Atmos::Asset::ShouldCreateAsset<::Atmos::Asset::ScriptAsset>(reliquary, fileName);
    }
}

namespace Inscription
{
    Scribe<::Atmos::Asset::ScriptAsset, BinaryArchive>::Table::Table()
    {
        MergeDataLinks({
            DataLink::Base(data.base) }
        );
    }
}