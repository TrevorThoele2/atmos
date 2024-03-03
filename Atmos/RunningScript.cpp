
#include "RunningScript.h"

#include "ObjectManager.h"

#include "ScriptEngineManager.h"
#include "AngelScriptAssert.h"
#include <angelscript.h>

#include "CurrentField.h"

namespace Atmos
{
    class RunningScript::Data
    {
    public:
        SourceReference source;

        asIScriptContext* context;

        bool isInitialized;

        Data(SourceReference source) : source(source), context(nullptr), isInitialized(false)
        {}

        Data(const Data& arg) : source(arg.source), context(nullptr), isInitialized(false)
        {}

        ~Data()
        {
            context->Release();
        }

        void Initialize()
        {
            if (isInitialized)
                return;

            auto engine = GetLocalObjectManager()->FindSystem<Scripting::EngineManager>()->Engine();
            context = engine->CreateContext();

            isInitialized = true;
        }
    };

    RunningScript::RunningScript(SourceReference source) :
        data(new Data(source)), suspended(false), hasBeenExecuted(false), executedThisFrame(false), source([this]() { return data->source; })
    {}

    RunningScript::RunningScript(const ::Inscription::Table<RunningScript>& table) :
        INSCRIPTION_TABLE_GET_BASE(Object), data(new Data(source)), source([this]() { return data->source; })
    {}

    void RunningScript::Resume()
    {
        if (!suspended)
            return;

        suspended = false;
    }

    void RunningScript::Suspend()
    {
        if (suspended)
            return;

        Scripting::AngelScriptAssert(data->context->Suspend());
        suspended = true;
    }

    bool RunningScript::IsSuspended() const
    {
        return suspended;
    }

    bool RunningScript::ShouldExecuteMain() const
    {
        return executeName.empty();
    }

    asIScriptContext* RunningScript::UnderlyingContext()
    {
        return data->context;
    }

    ObjectTypeDescription RunningScript::TypeDescription() const
    {
        return ObjectTraits<RunningScript>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<RunningScript>::typeName = "RunningScript";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::RunningScript)
    {

    }
}