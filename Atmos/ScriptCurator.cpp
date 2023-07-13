#include "ScriptCurator.h"

#include "ScriptFinished.h"

#include "CreateStopwatch.h"
#include "DiagnosticsStatistics.h"

#include <Arca/Destroy.h>

namespace Atmos::Scripting
{
    Curator::Curator(Init init, Manager& manager) :
        Arca::Curator(init),
        manager(&manager)
    {
        Owner().On<Arca::DestroyingKnown<Script>>(
            [this](const Arca::DestroyingKnown<Script>& signal)
            {
                if (&signal.index->data != current)
                    Owner().Raise(Finished{ signal.index, Quit{} });
            });
    }

    void Curator::Handle(const Work&)
    {
        auto stopwatch = Time::CreateRealStopwatch();
        
        const auto runningScripts = RunningScripts(Owner().Batch<Script>());
        for (auto& element : runningScripts)
            HandleScriptResult(DoExecute(element.script ? &element.script->data : nullptr), element.id);

        MutablePointer().Of<Diagnostics::Statistics>()->script.NewTime(stopwatch);
    }
    
    std::vector<CompiledModule> Curator::Handle(const Compile& command)
    {
        return manager->Compile(command.modules);
    }

    std::optional<Result> Curator::Handle(const Execute& command)
    {
        const auto script = command.script;
        const auto data = script ? &MutablePointer().Of(script)->data : nullptr;
        return HandleScriptResult(DoExecute(data), script.ID());
    }

    std::optional<Result> Curator::Handle(const ExecuteMaterial& command)
    {
        return DoExecute(command.data);
    }

    std::unique_ptr<Asset::Resource::Script> Curator::Handle(
        const Asset::Resource::Create<Asset::Resource::Script>& command)
    {
        return manager->CreateAssetResource(command.buffer, command.name);
    }
    
    auto Curator::RunningScripts(const Arca::Batch<Script>& batch) -> std::vector<RunningScript>
    {
        std::vector<RunningScript> runningScripts;
        runningScripts.reserve(batch.Size());
        for (auto script = batch.begin(); script != batch.end(); ++script)
            runningScripts.emplace_back(script.ID(), MutablePointer().Of<Script>(script.ID()));
        return runningScripts;
    }

    std::optional<Result> Curator::DoExecute(ScriptData* script)
    {
        if (!script->asset)
            return Quit{};

        manager->SetReliquary(&Owner());

        if (!script->resource)
            script->resource = manager->CreateScriptResource(
                *script->asset->Resource(), script->asset->Name(), script->executeName, script->parameters);

        const auto wasSuspended = script->isSuspended;
        script->isSuspended = false;
        auto result = wasSuspended ? script->resource->Resume() : script->resource->Execute();
        script->isSuspended = true;

        return result;
    }

    std::optional<Result> Curator::HandleScriptResult(const std::optional<Result>& result, Arca::RelicID id)
    {
        if (result)
        {
            const auto script = Owner().Find<Script>(id);
            current = &script->data;
            Owner().Raise(Finished{ script, *result });
            Owner().Do(Arca::Destroy<Script>(id));
            current = nullptr;
        }

        return result;
    }
}