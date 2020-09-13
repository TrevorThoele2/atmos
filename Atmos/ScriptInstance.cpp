#include "ScriptInstance.h"

#include <Arca/Reliquary.h>

#include "RunningScript.h"
#include "AllRunningScripts.h"
#include "ExecuteImmediately.h"

namespace Atmos::Script
{
    Instance::Instance(
        Init init,
        Arca::Index<Asset::Script> asset,
        Name executeName,
        Parameters parameters,
        Persistence persistence)
        :
        ClosedTypedRelic(init),
        asset(asset),
        executeName(executeName),
        parameters(parameters),
        persistence(persistence)
    {}

    Instance::Instance(Init init, Arca::Serialization) : ClosedTypedRelic(init)
    {}

    void Instance::ExecuteDeferred()
    {
        if (IsRunning())
            return;

        CreateRunningFromThis();
    }

    void Instance::ExecuteImmediately()
    {
        if (IsRunning())
            return;

        auto running = CreateRunningFromThis();
        Owner().Do(Script::ExecuteImmediately{ running });
    }

    Arca::Index<RunningScript> Instance::RunningForThis() const
    {
        if (!IsRunning())
            return {};

        return Arca::Index<AllRunningScripts>(Owner())->RunningScriptFor(ID());
    }

    bool Instance::IsRunning() const
    {
        return Arca::Index<AllRunningScripts>(Owner())->IsRunning(ID());
    }

    Arca::Index<RunningScript> Instance::CreateRunningFromThis()
    {
        return Owner().Do(Arca::Create<RunningScript> {
            Arca::Index<Instance>(ID(), Owner()),
            executeName,
            parameters,
            persistence });
    }
}