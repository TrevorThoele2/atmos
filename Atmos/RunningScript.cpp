#include "RunningScript.h"

#include "ObjectManager.h"
#include "AngelScriptSystem.h"

#include "AngelScriptAssert.h"
#include <angelscript.h>

namespace Atmos::Script
{
    class RunningScript::Data
    {
    public:
        SourceReference source;
        asIScriptContext* context;
        bool isInitialized;
    public:
        ObjectManager* objectManager;
    public:
        Data(SourceReference source) :
            source(source), context(nullptr), isInitialized(false), objectManager(nullptr)
        {}

        Data(const Data& arg) :
            source(arg.source), context(nullptr), isInitialized(false), objectManager(arg.objectManager)
        {}

        ~Data()
        {
            context->Release();
        }

        void Initialize()
        {
            if (isInitialized)
                return;

            auto engine = objectManager->FindSystem<Script::ScriptSystem>()->Engine();
            context = engine->CreateContext();

            isInitialized = true;
        }
    };

    RunningScript::RunningScript(ObjectManager& manager, SourceReference source) :
        Object(manager), data(new Data(source)), suspended(false), hasBeenExecuted(false), executedThisFrame(false),
        source([this]() { return data->source; })
    {
        data->objectManager = &manager;
    }

    RunningScript::RunningScript(const ::Inscription::BinaryTableData<RunningScript>& data) :
        Object(std::get<0>(data.bases)), data(new Data(source)), source([this]() { return this->data->source; })
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

        AngelScriptAssert(data->context->Suspend());
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
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Script::RunningScript>::typeName = "RunningScript";
}