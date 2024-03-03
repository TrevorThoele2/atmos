#pragma once

#include <unordered_map>

#include "ObjectSystem.h"
#include "ObjectBatch.h"
#include "ObjectID.h"
#include "ObjectReference.h"

#include "RunningScript.h"
#include "ScriptInstance.h"

namespace Atmos
{
    class LoggingSystem;

    class ScriptController : public ObjectSystem
    {
    public:
        typedef TypedObjectReference<RunningScript> RunningScriptReference;
        typedef ConstTypedObjectReference<ScriptInstance> ScriptInstanceReference;
    public:
        ScriptController(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(ScriptController);

        void ExecuteImmediately(RunningScriptReference reference);

        void ForceQuit(RunningScriptReference reference);

        RunningScriptReference Current();

        ObjectBatchSizeT Size() const;
    public:
        RunningScriptReference RunningScriptFor(ScriptInstanceReference scriptInstance) const;
        bool IsRunning(ScriptInstanceReference scriptInstance) const;
    private:
        void InitializeImpl() override;
    private:
        typedef ObjectBatch<RunningScript> RunningScripts;
        typedef RunningScripts::iterator RunningIterator;
        RunningScripts runningScripts;
        RunningIterator current;
    private:
        typedef std::unordered_map<ScriptInstanceReference, RunningScriptReference> RunningScriptMap;
        RunningScriptMap runningScriptMap;
    private:
        void WorkImpl() override;
    private:
        RunningIterator Find(RunningScriptReference reference);
        void Remove(RunningIterator itr);

        void LaunchOrRunScript(RunningIterator itr);
    private:
        void OnRunningScriptCreated(RunningScriptReference reference);
        void OnRunningScriptDestroyed(RunningScriptReference reference);
    private:
        LoggingSystem* FindLoggingSystem();
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::ScriptController)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}
