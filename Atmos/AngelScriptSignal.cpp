#include "AngelScriptSignal.h"

#include "CurrentExecutingScript.h"

namespace Atmos::Scripting::Angel
{
    namespace Detail
    {
        Arca::Index<Script> RetrieveCurrentExecuting(Arca::Reliquary& reliquary)
        {
            const auto currentRunning = reliquary.Find<CurrentExecutingScript>();
            return currentRunning
                ? reliquary.Find<Script>(currentRunning->id)
                : Arca::Index<Script>{};
        }
    }
}