#include "ScriptCurator.h"

#include "CurrentExecutingScript.h"
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
        this->manager->SetReliquary(&Owner());

        Owner().On<Arca::DestroyingKnown<Script>>(
            [this](const Arca::DestroyingKnown<Script>& signal)
            {
                const auto currentExecutingScript = Owner().Find<CurrentExecutingScript>()->id;
                if (currentExecutingScript != signal.index.ID())
                    Owner().Raise(Finished{ signal.index, Quit{} });
            });
    }

    void Curator::Handle(const Work&)
    {
        auto stopwatch = Time::CreateRealStopwatch();

        const auto currentExecutingScript = MutablePointer().Of<CurrentExecutingScript>();
        const auto runningScripts = ScriptsToRunning(Owner().Batch<Script>());
        for (auto& element : runningScripts)
            DoExecute(element.id, *currentExecutingScript);

        MutablePointer().Of<Diagnostics::Statistics>()->script.NewTime(stopwatch);
    }

    void Curator::Handle(const Suspend& command)
    {
        const auto script = MutablePointer().Of(command.script);
        if (!script || script->isSuspended)
            return;

        script->isSuspended = true;
        script->Resource()->Suspend();
    }

    std::vector<CompiledModule> Curator::Handle(const Compile& command)
    {
        return manager->Compile(command.modules);
    }

    std::optional<Result> Curator::Handle(const Execute& command)
    {
        const auto currentExecutingScript = MutablePointer().Of<CurrentExecutingScript>();

        return DoExecute(command.script.ID(), *currentExecutingScript);
    }

    std::unique_ptr<Asset::Resource::Script> Curator::Handle(
        const Asset::Resource::Create<Asset::Resource::Script>& command)
    {
        return manager->CreateAssetResource(command.buffer, command.name);
    }

    Curator::RunningScript::RunningScript(Arca::RelicID id, const Script* script) : id(id), script(script)
    {}

    Curator::RunningScript::RunningScript(const RunningScript& arg) = default;

    auto Curator::ScriptsToRunning(Arca::Batch<Script> batch) -> std::vector<RunningScript>
    {
        std::vector<RunningScript> runningScripts;
        runningScripts.reserve(batch.Size());
        for (auto script = batch.begin(); script != batch.end(); ++script)
            runningScripts.emplace_back(script.ID(), &*script);
        return runningScripts;
    }

    std::optional<Result> Curator::DoExecute(Arca::RelicID id, CurrentExecutingScript& currentExecutingScript)
    {
        currentExecutingScript.id = id;

        auto script = MutablePointer().Of<Script>(id);
        if (!script->asset)
        {
            currentExecutingScript.id = Arca::nullRelicID;
            Owner().Do(Arca::Destroy<Script>(id));
            return Quit{};
        }

        if (!script->Resource())
        {
            auto resource = manager->CreateScriptResource(
                *script->asset->Resource(), script->asset->Name(), script->executeName, script->parameters);
            script->Setup(std::move(resource));
        }

        std::optional<Result> result;

        if (script->isSuspended)
        {
            script->isSuspended = false;
            result = script->Resource()->Resume();
        }
        else
            result = script->Resource()->Execute();

        if (result)
        {
            Owner().Raise(Finished{ Owner().Find<Script>(id), *result });
            Owner().Do(Arca::Destroy<Script>(id));
        }

        currentExecutingScript.id = Arca::nullRelicID;

        return result;
    }
}