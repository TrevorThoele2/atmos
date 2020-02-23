#include "ScriptController.h"

#include "AngelScriptUtility.h"
#include "AngelScriptResultVerification.h"
#include <angelscript.h>

#include <Arca/Reliquary.h>
#include <Arca/Actualization.h>
#include "Log.h"

namespace Atmos::Script
{
    ScriptController::ScriptController(Init init) :
        Curator(init), allRunningScripts(init.owner)
    {
        Owner().ExecuteOn<Arca::CreatedKnown<RunningScript>>(
            [](const Arca::CreatedKnown<RunningScript>& signal)
            {
                
            });

        Owner().ExecuteOn<Arca::DestroyingKnown<RunningScript>>(
            [](const Arca::DestroyingKnown<RunningScript>& signal)
            {
                
            });
    }

    void ScriptController::Work()
    {
        
    }

    void ScriptController::Handle(const ExecuteImmediately& command)
    {
        
    }

    void ScriptController::Handle(const ForceQuit& command)
    {
        
    }

    void ScriptController::LaunchOrRunScript(RunningScript& script)
    {

    }
}