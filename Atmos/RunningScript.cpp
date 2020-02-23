#include "RunningScript.h"

#include <Arca/Reliquary.h>
#include "AngelScriptCurator.h"

#include "AngelScriptResultVerification.h"
#include <angelscript.h>

namespace Atmos::Script
{
    class RunningScript::Data
    {
    public:
        asIScriptContext* context = nullptr;
    public:
        ~Data()
        {
            if (context)
                context->Release();
        }

        void Initialize(asIScriptEngine* engine)
        {
            if (context)
                return;

            context = engine->CreateContext();
        }
    };

    RunningScript::RunningScript(Init init) : ClosedTypedRelic(init)
    {}

    RunningScript::RunningScript(
        Init init,
        Arca::Index<Source> source,
        const Name& executeName,
        const Parameters& parameters,
        const Persistence& persistence)
        :
        ClosedTypedRelic(init),
        source(source), executeName(executeName), parameters(parameters), persistence(persistence),
        data(std::make_unique<Data>())
    {}

    RunningScript::RunningScript(const RunningScript& arg) :
        ClosedTypedRelic(arg),
        source(arg.source),
        data(std::make_unique<Data>(*arg.data))
    {}

    RunningScript::~RunningScript() = default;

    RunningScript& RunningScript::operator=(const RunningScript& arg)
    {
        source = arg.source;
        data = std::make_unique<Data>(*arg.data);
        return *this;
    }

    void RunningScript::Resume()
    {
        if (!isSuspended)
            return;

        isSuspended = false;
    }

    void RunningScript::Suspend()
    {
        if (isSuspended)
            return;

        Angel::VerifyResult(data->context->Suspend());
        isSuspended = true;
    }

    bool RunningScript::IsSuspended() const
    {
        return isSuspended;
    }

    bool RunningScript::ShouldExecuteMain() const
    {
        return executeName.empty();
    }

    asIScriptContext* RunningScript::UnderlyingContext()
    {
        return data->context;
    }
}