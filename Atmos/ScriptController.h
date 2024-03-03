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
}

namespace Atmos::Script
{
    class ScriptController : public ObjectSystem
    {
    public:
        typedef TypedObjectReference<RunningScript> RunningScriptReference;
        typedef ConstTypedObjectReference<ScriptInstance> ScriptInstanceReference;
    public:
        ScriptController(ObjectManager& manager);

        void ExecuteImmediately(RunningScriptReference reference);

        void ForceQuit(RunningScriptReference reference);

        RunningScriptReference Current();

        ObjectBatchSizeT Size() const;
    public:
        RunningScriptReference RunningScriptFor(ScriptInstanceReference scriptInstance) const;
        bool IsRunning(ScriptInstanceReference scriptInstance) const;
    protected:
        void InitializeImpl() override;
        void WorkImpl() override;
    private:
        typedef ObjectBatch<RunningScript> RunningScripts;
        typedef RunningScripts::iterator RunningIterator;
        RunningScripts runningScripts;
        RunningIterator current;
    private:
        typedef std::unordered_map<ScriptInstanceReference, RunningScriptReference> RunningScriptMap;
        RunningScriptMap runningScriptMap;
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
    template<>
    class Scribe<::Atmos::Script::ScriptController, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Script::ScriptController, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}
