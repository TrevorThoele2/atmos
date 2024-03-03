#include "AngelScriptSignal.h"

#include "CurrentExecutingScript.h"

namespace Atmos::Scripting::Angel
{
    namespace Detail
    {
        Arca::Index<Script> RetrieveCurrentExecuting(Arca::Reliquary& reliquary)
        {
            const auto currentRunning = Arca::Index<CurrentExecutingScript>(reliquary);
            return currentRunning
                ? Arca::Index<Script>(currentRunning->id, reliquary)
                : Arca::Index<Script>{};
        }
    }
}