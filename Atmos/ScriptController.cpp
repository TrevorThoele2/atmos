#include "ScriptController.h"

#include "AngelScriptUtility.h"
#include "AngelScriptResultVerification.h"
#include <angelscript.h>

#include <Arca/Reliquary.h>
#include <Arca/Actualization.h>
#include "DebugStatistics.h"
#include "Log.h"

namespace Atmos::Script
{
    void ScriptController::InitializeImplementation()
    {
        Owner().ExecuteOn<Arca::Created>(
            [](const Arca::Created& signal)
            {
                auto actualized = Arca::Actualize<RunningScript>(signal.handle);
                if (!actualized)
                    return;

                //allRunningScripts->Add(actualized, actualized);
            });

        Owner().ExecuteOn<Arca::Destroying>(
            [](const Arca::Destroying& signal)
            {
                auto actualized = Arca::Actualize<RunningScript>(signal.handle);
                if (!actualized)
                    return;

                //allRunningScripts->Remove(actualized);
            });

        allRunningScripts = Arca::GlobalIndex<AllRunningScripts>(Owner());

        debugStatistics = Arca::GlobalIndex<Debug::Statistics>(Owner());
    }

    void ScriptController::WorkImplementation(Stage& stage)
    {

    }

    void ScriptController::LaunchOrRunScript(RunningScript& script)
    {

    }
}